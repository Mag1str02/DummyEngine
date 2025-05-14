#pragma once

#include "DummyEngine/Core/UI/UIWidgetSystem.h"
#include "DummyEngine/Core/UI/UIWidgetComponent.h"
#include "DummyEngine/Core/UI/UIContainer.h"
#include "DummyEngine/Core/UI/UIText.h"
#include "DummyEngine/Core/UI/UIButton.h"
#include "DummyEngine/Core/UI/UICheckbox.h"
#include "DummyEngine/Core/UI/UITextField.h"
#include <imgui.h>

namespace DummyEngine {

class UIWidgetEditor {
public:
    static void DrawWidgetHierarchy(Entity widget);
    static void DrawWidgetProperties(Entity widget);
    static void DrawWidgetCreationMenu(Entity parent = Entity());
    static void DrawWidgetDragDrop(Entity source, Entity target);

private:
    static void DrawWidgetNode(Entity widget);
    static void DrawWidgetTypeProperties(Entity widget);
    static void DrawContainerProperties(Entity widget);
    static void DrawTextProperties(Entity widget);
    static void DrawButtonProperties(Entity widget);
    static void DrawCheckboxProperties(Entity widget);
    static void DrawTextFieldProperties(Entity widget);
};

} // namespace DummyEngine 