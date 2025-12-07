#include "language.h"

#include <qapplication.h>
#include <qevent.h>

#include <easy_translate.hpp>

#include "def.h"

static QString getLanguageIdStr(LanguageID langId)
{
    switch (langId)
    {
        case LANG_EN:   return "En";
        case LANG_ZH:   return "Zh";
        default:        return "";
    }
}

bool setLanguage(LanguageID langId)
{
    easytr::setLanguages(APP_LANGUAGES_FILENAME);
    if (easytr::languages().empty())
        qDebug() << "[Warning] Invalid Languages file";

    auto langIdStr = getLanguageIdStr(langId).toStdString();
    if (easytr::hasLanguage(langIdStr))
    {
        if (easytr::setCurrentLanguage(langIdStr))
            qDebug() << "[Info] Success to change the language to: " << langIdStr.c_str();
        else
            qDebug() << "[Warning] Failed to change the language to: " << langIdStr.c_str();
    }
    // Fallthrough, rollback
    else
    {
        qDebug() << "[Info] Expected language is miss, start rollback the language";
        if (easytr::languages().empty())
        {
            qDebug() << "[Warning] Not find any language";
            return false;
        }
        else
        {
            auto rollbackId = easytr::languages().getIds().front();
            if (easytr::setCurrentLanguage(rollbackId))
                qDebug() << "[Info] Success to rollback the language to: " << rollbackId.c_str();
            else
                qDebug() << "[Warning] Failed to rollback the language to: " << rollbackId.c_str();
        }
    }

    QEvent event(QEvent::Type::LanguageChange);
    qApp->sendEvent(qApp, &event);

    return true;
}
