#pragma once

#include <qdatetime.h>
#include <qstring.h>

enum class NumStringAlign
{
    LEFT,   /** 对齐至左侧。（将在字符串右侧补零） */
    RIGHT   /** 对齐至右侧。（将在字符串左侧补零） */
};

/**
 * @brief 获取长度对齐的数字字符串，若数字字符串长度不满足指定长度将按照指定方式补零对齐。
 * @param size 若数字字符串长度小于`size`将进行对齐操作。
 * @param align 对齐方式。
 * @sa #NumStringAlign
 */
QString getJustifiedNumString(int num, int size = 2, NumStringAlign align = NumStringAlign::RIGHT);

/**
 * @brief 获取指定秒对应的时间字符串。
 * @note 时间字符串将满足此格式：`HHH:mm:SS`。
 */
QString getTimeStringFromSecs(uint64_t secs);

/**
 * @brief 将`DateTime`转换成字符串。
 * @note 转换得到的字符串将满足此格式：`YY-MM-DD HH:m:SS`。
 */
QString dateTime2String(const QDateTime& dt);

/**
 * @brief 将字符串转换成`DateTime`。
 * @note 字符串需满足此格式：`YY-MM-DD HH:mm:SS`。
 */
QDateTime string2DateTime(const QString& str);

/**
 * @brief 将`DateTime`转换成仅含合法路径字符的字符串。
 * @note 转换得到的字符串将满足此格式：`YY-MM-DD HHmmSS`。
 */
QString dateTime2StringForFilename(const QDateTime& dt);
