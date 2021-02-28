set(gui_sources
    ./DrawRuler.cpp
    ./DrawRuler.hpp
    ./FlowLayout.cpp
    ./FlowLayout.hpp
    ./GridTickModel.cpp
    ./GridTickModel.hpp
    ./SqlTableView.cpp
    ./SqlTableView.hpp
    ./StandardGridTickModel.hpp
    ./UInt64Mapper.cpp
    ./UInt64Mapper.hpp
    ./UndoAction.cpp
    ./UndoAction.hpp
    ./UndoBase.cpp
    ./UndoBase.hpp
    ./UndoCallback.hpp

    ./edit/CheckBox.cpp
    ./edit/CheckBox.hpp
    ./edit/ColorChip.cpp
    ./edit/ColorChip.cpp
    ./edit/ComboBox.cpp
    ./edit/ComboBox.hpp
    ./edit/ColorCombo.cpp
    ./edit/ColorCombo.hpp
    ./edit/DateEdit.cpp
    ./edit/DateEdit.hpp
    ./edit/DateTimeEdit.cpp
    ./edit/DateTimeEdit.hpp
    ./edit/DateTimeWidget.cpp
    ./edit/DateTimeWidget.hpp
    ./edit/DateWidget.cpp
    ./edit/DateWidget.hpp
    ./edit/DirBox.cpp
    ./edit/DirBox.hpp
    ./edit/DoubleSpinBox.cpp
    ./edit/DoubleSpinBox.hpp
    ./edit/EnumCombo.cpp
    ./edit/EnumCombo.hpp
    ./edit/FileBox.cpp
    ./edit/FileBox.hpp
    ./edit/GenericEnumCombo.hpp
    ./edit/LineEdit.cpp
    ./edit/LineEdit.hpp
    ./edit/SpinBox.cpp
    ./edit/SpinBox.hpp
    ./edit/StringSetCheck.cpp
    ./edit/StringSetCheck.hpp
    ./edit/TextEdit.cpp
    ./edit/TextEdit.hpp


    ./validator/IdentifierValidator.cpp
    ./validator/IdentifierValidator.hpp
    ./validator/KeyValidator.cpp
    ./validator/KeyValidator.hpp
    ./validator/NoSpacesValidator.cpp
    ./validator/NoSpacesValidator.hpp
    ./validator/XmlIdentifierValidator.cpp
    ./validator/XmlIdentifierValidator.hpp
)

add_library(yqgui SHARED ${gui_sources})
target_link_libraries(yqgui PUBLIC yqutil yqmeta Qt5::Gui Qt5::Widgets Qt5::Sql)

