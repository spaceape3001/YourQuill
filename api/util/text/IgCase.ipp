#pragma once

bool    IgCase::operator()(const String&a, const String&b) const
{
    return is_less( compare_igCase(a,b));
}


bool    IgCase::operator()(const StringView&a, const StringView&b) const
{
    return is_less( compare_igCase(a,b));
}


bool    IgCase::operator()(const QString&a, const QString&b) const
{
    return is_less( compare_igCase(a,b));
}

bool    IgCase::operator()(const QByteArray&a, const QByteArray&b) const
{
    return is_less( compare_igCase(a,b));
}

bool    RevIgCase::operator()(const String&a, const String&b) const
{
    return is_greater( compare_igCase(a,b));
}

bool    RevIgCase::operator()(const StringView&a, const StringView&b) const
{
    return is_greater( compare_igCase(a,b));
}

bool    RevIgCase::operator()(const QString&a, const QString&b) const
{
    return is_greater( compare_igCase(a,b));
}

bool    RevIgCase::operator()(const QByteArray&a, const QByteArray&b) const
{
    return is_greater( compare_igCase(a,b));
}
