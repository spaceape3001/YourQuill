#pragma once

namespace yq {

    ByteArray::ByteArray(const Vector<char>&dat) : m_data(dat)
    {
    }

    ByteArray::ByteArray(Vector<char>&&dat) : m_data(std::move(dat))
    {
    }

    ByteArray::ByteArray(const QByteArray& ba) : m_data(ba.data(), ba.size())
    {
    }

    QByteArray          ByteArray::qBytes() const
    {
        return QByteArray(m_data.data(), m_data.size());
    }

    void            ByteArray::setData(Vector<char>&&mv)
    {
        m_data  = std::move(mv);
    }

}
