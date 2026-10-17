#include <cstdio>
#include <cstdlib>
#include <difont/difont.h>
#include "core/filesystem/FileSystem.h"
#include "ui/Font.h"
#include "ui/fonts/FontLoader.h"

static void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

struct TestFont : hd::ui::Font {
    TestFont() : hd::ui::Font(0) {}
};

int main(int argc, char** argv) {
    require(argc == 2, "pass the VastApps resource root");
    hd::FileSystem::Init(argv[1]);
    auto loader = hd::ui::FontLoader::Instance();
    auto regular = loader->FindTextureFont("resources/fonts/Carlito-Regular.ttf", 24);
    auto bold = loader->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 24);
    auto fallback = loader->FindTextureFont("DEFAULT", 24);
    require(regular && !regular->Error(), "Carlito Regular loads with its Unicode map");
    require(bold && !bold->Error(), "Carlito Bold loads with its Unicode map");
    require(fallback && !fallback->Error(), "embedded fallback loads with its Unicode map");
    require(loader->FindTextureFont("resources/fonts/Carlito-Regular.ttf", 24) == regular,
            "same face and size reuse the cache entry");
    require(loader->FindTextureFont("resources/fonts/Carlito-Regular.ttf", 36) != regular,
            "different face sizes have distinct cache entries");
    TestFont component;
    component.SetFont(bold);
    component.SetFaceSize(18);
    require(!bold->Error(), "resizing preserves the valid character map");
    auto bounds = bold->BBox("VastBuilder");
    require(bounds.Upper().X() > bounds.Lower().X(), "UI text has a nonempty outline");
    auto fallbackBounds = fallback->BBox("Fallback");
    require(fallbackBounds.Upper().X() > fallbackBounds.Lower().X(), "fallback text has a nonempty outline");
    std::puts("Font loading, fallback, Unicode resizing, and cache regression checks passed.");
}
