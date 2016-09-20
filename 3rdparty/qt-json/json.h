/**
 * QtJson - A simple class for parsing JSON data into a QVariant hierarchies and vice-versa.
 * Copyright (C) 2011  Eeli Reilin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file json.h
 */

#ifndef JSON_H
#define JSON_H

#include <QVariant>
#include <QString>


/**
 * \namespace QtJson
 * \brief A JSON data parser
 *
 * Json parses a JSON data into a QVariant hierarchy.
 */
namespace QtJson {
    const qlonglong NUMBER_MAX_SAFE_INTEGER = 9007199254740991;

    typedef QVariantMap JsonObject;
    typedef QVariantList JsonArray;

    /**
     * Clone a JSON object (makes a deep copy)
     *
     * \param data The JSON object
     */
    QVariant clone(const QVariant &data);

    /**
     * Insert value to JSON object (QVariantMap)
     *
     * \param v The JSON object
     * \param key The key
     * \param value The value
     */
    void insert(QVariant &v, const QString &key, const QVariant &value);

    /**
     * Append value to JSON array (QVariantList)
     *
     * \param v The JSON array
     * \param value The value
     */
    void append(QVariant &v, const QVariant &value);

    /**
     * Parse a JSON string
     *
     * \param json The JSON data
     */
    QVariant parse(const QString &json);

    /**
     * Parse a JSON string
     *
     * \param json The JSON data
     * \param success The success of the parsing
     */
    QVariant parse(const QString &json, bool &success);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     *
     * \return QByteArray Textual JSON representation in UTF-8
     */
    QByteArray serialize(const QVariant &data);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     * \param success The success of the serialization
     *
     * \return QByteArray Textual JSON representation in UTF-8
     */
    QByteArray serialize(const QVariant &data, bool &success);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     *
     * \return QString Textual JSON representation
     */
    QString serializeStr(const QVariant &data);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     * \param success The success of the serialization
     *
     * \return QString Textual JSON representation
     */
    QString serializeStr(const QVariant &data, bool &success);

    /**
     * This method sets date(time) format to be used for QDateTime::toString
     * If QString is empty, Qt::TextDate is used.
     *
     * \param format The JSON data generated by the parser.
     */
    void setDateTimeFormat(const QString& format);
    void setDateFormat(const QString& format);

    /**
     * This method gets date(time) format to be used for QDateTime::toString
     * If QString is empty, Qt::TextDate is used.
     */
    QString getDateTimeFormat();
    QString getDateFormat();

    /**
     * QVariant based Json object
     */
    class Object : public QVariant {
        template<typename T>
        Object& insertKey(Object* ptr, const QString& key) {
            T* p = (T*)ptr->data();
            if (!p->contains(key)) p->insert(key, QVariant());
            return *reinterpret_cast<Object*>(&p->operator[](key));
        }
        template<typename T>
        void removeKey(Object *ptr, const QString& key) {
            T* p = (T*)ptr->data();
            p->remove(key);
        }
    public:
        Object() : QVariant() {}
        Object(const Object& ref) : QVariant(ref) {}

        Object& operator=(const QVariant& rhs) {
            setValue(rhs);
            return *this;
        }
        Object& operator[](const QString& key) {
            if (type() == QVariant::Map)
                return insertKey<QVariantMap>(this, key);
            else if (type() == QVariant::Hash)
                return insertKey<QVariantHash>(this, key);

            setValue(QVariantMap());

            return insertKey<QVariantMap>(this, key);
        }
        const Object& operator[](const QString& key) const {
            return const_cast<Object*>(this)->operator[](key);
        }
        void remove(const QString& key) {
            if (type() == QVariant::Map)
                removeKey<QVariantMap>(this, key);
            else if (type() == QVariant::Hash)
                removeKey<QVariantHash>(this, key);
        }
    };
}

#endif //JSON_H
