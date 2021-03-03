set(gui_sources

    ./UInt64Mapper.cpp
    ./UInt64Mapper.hpp

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
    ./edit/UShortSetEdit.cpp
    ./edit/UShortSetEdit.hpp

    ./layout/FlowLayout.cpp
    ./layout/FlowLayout.hpp

    ./main/MainWin.cpp
    ./main/MainWin.hpp
    ./main/SimpleEditor.cpp
    ./main/SimpleEditor.hpp
    ./main/SubWin.cpp
    ./main/SubWin.hpp
    ./main/UndoMain.cpp
    ./main/UndoMain.hpp
    ./main/UndoWin.cpp
    ./main/UndoWin.hpp

    ./undo/UndoAction.cpp
    ./undo/UndoAction.hpp
    ./undo/UndoBase.cpp
    ./undo/UndoBase.hpp
    ./undo/UndoCallback.hpp

    ./validator/IdentifierValidator.cpp
    ./validator/IdentifierValidator.hpp
    ./validator/KeyValidator.cpp
    ./validator/KeyValidator.hpp
    ./validator/NoSpacesValidator.cpp
    ./validator/NoSpacesValidator.hpp
    ./validator/UShortSetValidator.cpp
    ./validator/UShortSetValidator.hpp
    ./validator/XmlIdentifierValidator.cpp
    ./validator/XmlIdentifierValidator.hpp

    ./view/SimpleTable.cpp
    ./view/SimpleTable.hpp
    ./view/SqlTableView.cpp
    ./view/SqlTableView.hpp


    ./widget/DrawRuler.cpp
    ./widget/DrawRuler.hpp
    ./widget/GridTickModel.cpp
    ./widget/GridTickModel.hpp
    ./widget/FormWidget.cpp
    ./widget/FormWidget.hpp
    ./widget/StandardGridTickModel.hpp
    ./widget/TabWidget.cpp
    ./widget/TabWidget.hpp
    ./widget/TabWidgetBar.cpp
    ./widget/TabWidgetBar.hpp
    
)

add_library(yqgui SHARED ${gui_sources})
target_link_libraries(yqgui 
    PUBLIC 
        yqutil 
        yqmeta 
        Qt5::Gui 
        Qt5::Sql
        Qt5::Widgets
)

