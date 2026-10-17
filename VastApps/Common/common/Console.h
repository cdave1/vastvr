#ifndef _VAST_APPS_CONSOLE_H_
#define _VAST_APPS_CONSOLE_H_

#include "AppPrototype.h"

class Console {
private:

    hd::Vector<std::string> m_logEntries;

    hd::Entity *m_console;

    hd::Entity *m_log;


public:

    Console(hd::EntityPool *entityPool);

    void AddAppItem(const std::string &appId, const std::string &iconPath);

    void ToggleConsole(hd::Entity *camera);

    void OnLog(const std::string &log);

    void OnLogAlarm();

private:

    void ShowConsole(hd::Entity *camera);

    void HideConsole(hd::Entity *camera);

};

#endif
