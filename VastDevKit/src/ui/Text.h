#ifndef _VAST_UI_TEXT_H_
#define _VAST_UI_TEXT_H_

#include <unordered_map>
#include "core/component/Component.h"
#include <Vast/VastMath.h>
#include "ui/Languages.h"

template<class T> class ComponentPool;

namespace hd {
    namespace ui {

        class Text : public Component {
        private:

            std::string m_key;

            std::unordered_map<hd::ui::LanguageCode, std::string> m_strings;


        protected:

            friend class ComponentPool<Text>;

            Text(unsigned int ID);


        public:

            std::string GetKey() const;

            void SetKey(const std::string &key);

            void SetKeyFormatted(const char *fmt, ...);

            const std::string & GetText(hd::ui::LanguageCode language = hd::ui::LanguageCodes::ENGLISH) const;

            void SetText(const std::string &text);

            void SetText(hd::ui::LanguageCode language, const std::string &text);

            void SetTextFormatted(hd::ui::LanguageCode language, const char *fmt, ...);

            void SetTextFormatted(const char *fmt, ...);

        };
    }
}

#endif