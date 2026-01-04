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
            qDebug() << "[Info] Successfully set language to: " << langIdStr.c_str();
        else
            qDebug() << "[Warning] Failed to set language to: " << langIdStr.c_str();
    }
    // Fallthrough, fallback
    else
    {
        qDebug() << "[Info] Expected language is missing, try fall back to the default language";
        if (easytr::languages().empty())
        {
            qDebug() << "[Warning] Not find any language";
            return false;
        }
        else
        {
            auto fallbackId = easytr::languages().getIds().front();
            if (easytr::setCurrentLanguage(fallbackId))
                qDebug() << "[Info] Successfully fall back to language: " << fallbackId.c_str();
            else
                qDebug() << "[Warning] Failed to fall back to language: " << fallbackId.c_str();
        }
    }

    QEvent event(QEvent::Type::LanguageChange);
    qApp->sendEvent(qApp, &event);

    return true;
}
