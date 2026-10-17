#include "Text.h"

hd::ui::Text::Text(unsigned int ID) : hd::Component(ID), m_key("(Undefined)"), m_strings() {
}


std::string hd::ui::Text::GetKey() const {
    return m_key;
}


void hd::ui::Text::SetKey(const std::string &key) {
    m_key = key;
}


void hd::ui::Text::SetKeyFormatted(const char *fmt, ...) {
    va_list args;
    static char key[2048];
    va_start(args, fmt);
#ifdef WIN32
    (void)vsnprintf_s(key, sizeof(key), fmt, args);
#else
    (void)vsnprintf(key, sizeof(key), fmt, args);
#endif
    va_end(args);
    key[sizeof(key) - 1] = '\0';
    SetKey(std::string(key));
}


const std::string & hd::ui::Text::GetText(hd::ui::LanguageCode language) const {
    if (m_strings.count(language) == 0) {
        hd::log::e("No string entry for language: %s\n", language.c_str());
        return m_key;
    } else {
        return m_strings.at(language);
    }
}


void hd::ui::Text::SetText(const std::string &text) {
    SetText(hd::ui::LanguageCodes::ENGLISH, text);
}


void hd::ui::Text::SetText(hd::ui::LanguageCode language, const std::string &text) {
    m_strings[language] = text;
}


void hd::ui::Text::SetTextFormatted(hd::ui::LanguageCode language, const char *fmt, ...) {
    va_list args;
    static char text[2048];
    va_start(args, fmt);
    (void)vsnprintf(text, sizeof(text), fmt, args);
    va_end(args);
    text[sizeof(text) - 1] = '\0';
    SetText(language, std::string(text));
}


void hd::ui::Text::SetTextFormatted(const char *fmt, ...) {
    va_list args;
    static char text[2048];
    va_start(args, fmt);
    (void)vsnprintf(text, sizeof(text), fmt, args);
    va_end(args);
    text[sizeof(text) - 1] = '\0';
    SetText(hd::ui::LanguageCodes::ENGLISH, std::string(text));
}
