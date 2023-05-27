#include "ImGuiCustom.h"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

void ImGui::hotkey(const char* label, KeyBind& key, float samelineOffset, const ImVec2& size) noexcept
{
    static bool waiting = true;

    ImGui::PushID(label);

    if (!waiting)
    {
        PushStyleColor(ImGuiCol_Button, GetColorU32(ImGuiCol_ButtonActive));
        Button("Press a key...", size);
        PopStyleColor();
        GetCurrentContext()->ActiveIdAllowOverlap = true;
        if ((!IsItemHovered() && GetIO().MouseClicked[0]) || key.setToPressedKey())
            waiting = true;
    }
    else
    {
        if (key.keyMode == KeyMode::Always)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
            ImGui::ButtonEx("On", {}, ImGuiButtonFlags_Disabled);
            ImGui::PopStyleColor();
        }
        else if (key.keyMode == KeyMode::Off)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
            ImGui::ButtonEx("Off", {}, ImGuiButtonFlags_Disabled);
            ImGui::PopStyleColor();
        }
        else if (key.isSet())
        {
            if (ImGui::Button(key.toString(), size))
                waiting = false;
        }
        else
        {
            if (ImGui::Button("Bind"))
                waiting = false;
        }

        if (ImGui::BeginPopup("##mode", ImGuiWindowFlags_AlwaysUseWindowPadding))
        {
            bool selected = key.keyMode == KeyMode::Off;
            ImGui::Selectable("Off", &selected);
            if (selected) {
                key.keyMode = KeyMode::Off;
                key.reset();
            }

            selected = key.keyMode == KeyMode::Always;
            ImGui::Selectable("Always", &selected);
            if (selected)
                key.keyMode = KeyMode::Always;

            selected = key.keyMode == KeyMode::Hold;
            ImGui::Selectable("Hold", &selected);
            if (selected)
                key.keyMode = KeyMode::Hold;

            selected = key.keyMode == KeyMode::Toggle;
            ImGui::Selectable("Toggle", &selected);
            if (selected)
                key.keyMode = KeyMode::Toggle;

            ImGui::EndPopup();
        }
        else if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Right click for options");

            if (ImGui::GetIO().MouseClicked[1])
                ImGui::OpenPopup("##mode");
        }
    }

    ImGui::SameLine();

    ImGui::AlignTextToFramePadding();
    if (std::strncmp(label, "##", 2))
        ImGui::TextUnformatted(label, std::strstr(label, "##"));
    ImGui::PopID();
}

void ImGui::textUnformattedCentered(const char* text) noexcept
{
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
    ImGui::TextUnformatted(text);
}

void ImGui::progressBarFullWidth(float fraction, float height) noexcept
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(ImVec2{ -1, 0 }, CalcItemWidth(), height + style.FramePadding.y * 2.0f);
    ImRect bb(pos, ImVec2{ pos.x + size.x, pos.y + size.y });
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    fraction = ImSaturate(fraction);
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));

    if (fraction == 0.0f)
        return;

    const ImVec2 p0{ ImLerp(bb.Min.x, bb.Max.x, 0.0f), bb.Min.y };
    const ImVec2 p1{ ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y };

    const float x0 = ImMax(p0.x, bb.Min.x);
    const float x1 = ImMin(p1.x, bb.Max.x);

    window->DrawList->PathLineTo({ x0, p1.y });
    window->DrawList->PathLineTo({ x0, p0.y });
    window->DrawList->PathLineTo({ x1, p0.y });
    window->DrawList->PathLineTo({ x1, p1.y });
    window->DrawList->PathFillConvex(GetColorU32(ImGuiCol_PlotHistogram));
}

void ImGuiCustom::classicHotkey(const char* label, KeyBind& key, float samelineOffset, const ImVec2& size) noexcept
{
    static bool waiting = true;
    ImGui::PushID(label);

    ImGui::TextUnformatted(label);
    ImGui::SameLine(samelineOffset);

    if (!waiting) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        ImGui::Button("Press a key...", size);
        ImGui::PopStyleColor();

        ImGui::GetCurrentContext()->ActiveIdAllowOverlap = true;
        if ((!ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[0]) || key.setToPressedKey())
            waiting = true;
    }
    else if (ImGui::Button(key.toString(), size)) {
        waiting = false;
    }

    ImGui::PopID();
}

void ImGuiCustom::colorPicker(const char* name, float color[3], float* alpha, bool* rainbow, float* rainbowSpeed, bool* enable, float* thickness, float* rounding, bool* outline) noexcept
{
    ImGui::PushID(name);
    if (enable) {
        ImGui::Checkbox("##check", enable);
        ImGui::SameLine(0.0f, 5.0f);
    }
    bool openPopup = ImGui::ColorButton("##btn", ImVec4{ color[0], color[1], color[2], alpha ? *alpha : 1.0f }, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview);
    if (ImGui::BeginDragDropTarget()) {
        if (alpha) {
            if (const auto payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F)) {
                std::copy((float*)payload->Data, (float*)payload->Data + 3, color);
                *alpha = 1.0f;
            }
            if (const auto payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                std::copy((float*)payload->Data, (float*)payload->Data + 4, color);
        }
        else {
            if (const auto payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                std::copy((float*)payload->Data, (float*)payload->Data + 3, color);
            if (const auto payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                std::copy((float*)payload->Data, (float*)payload->Data + 3, color);
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::TextUnformatted(name);

    if (openPopup)
        ImGui::OpenPopup("##popup");

    if (ImGui::BeginPopup("##popup")) {
        if (alpha) {
            float col[]{ color[0], color[1], color[2], *alpha };
            ImGui::ColorPicker4("##picker", col, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar);
            color[0] = col[0];
            color[1] = col[1];
            color[2] = col[2];
            *alpha = col[3];
        }
        else {
            ImGui::ColorPicker3("##picker", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        }

        ImGui::SameLine();

        if (rainbow || rainbowSpeed || thickness || rounding) {
            ImGui::SameLine();
            if (ImGui::BeginChild("##child", { 150.0f, 0.0f })) {
                if (rainbow)
                    ImGui::Checkbox("Rainbow", rainbow);
                ImGui::PushItemWidth(150.0f);
                if (rainbowSpeed)
                    ImGui::DragFloat("##speed", rainbowSpeed, 0.1f, -100.0f, 100.0f, "Speed %.1f");

                if (rounding || thickness)
                    ImGui::Separator();

                if (rounding) {
                    ImGui::DragFloat("##rounding", rounding, 0.1f, 0.0f, 100.0f, "Corner %.1f");
                    *rounding = std::max(*rounding, 0.0f);
                }

                if (thickness) {
                    ImGui::DragFloat("##thickness", thickness, 0.1f, 1.0f, 10.0f, "Thick %.2f");
                    *thickness = std::max(*thickness, 1.0f);
                }

                ImGui::PopItemWidth();
            }
            ImGui::EndChild();
        }
        ImGui::EndPopup();
    }
    ImGui::PopID();
}

void ImGuiCustom::colorPicker(const char* name, ColorToggle3& colorConfig) noexcept
{
    colorPicker(name, colorConfig.color.data(), nullptr, &colorConfig.rainbow, &colorConfig.rainbowSpeed, &colorConfig.enabled);
}

void ImGuiCustom::colorPicker(const char* name, Color4& colorConfig, bool* enable, float* thickness) noexcept
{
    colorPicker(name, colorConfig.color.data(), &colorConfig.color[3], &colorConfig.rainbow, &colorConfig.rainbowSpeed, enable, thickness);
}

void ImGuiCustom::colorPicker(const char* name, ColorToggle& colorConfig) noexcept
{
    colorPicker(name, colorConfig.color.data(), &colorConfig.color[3], &colorConfig.rainbow, &colorConfig.rainbowSpeed, &colorConfig.enabled);
}

void ImGuiCustom::colorPicker(const char* name, ColorToggleOutline& colorConfig, bool* enable, float* thickness) noexcept
{
    colorPicker(name, colorConfig.color.data(), &colorConfig.color[3], &colorConfig.rainbow, &colorConfig.rainbowSpeed, &colorConfig.enabled, thickness, nullptr, &colorConfig.outline);
}

void ImGuiCustom::colorPicker(const char* name, ColorToggleRounding& colorConfig) noexcept
{
    colorPicker(name, colorConfig.color.data(), &colorConfig.color[3], &colorConfig.rainbow, &colorConfig.rainbowSpeed, &colorConfig.enabled, nullptr, &colorConfig.rounding);
}

void ImGuiCustom::colorPicker(const char* name, ColorToggleThickness& colorConfig) noexcept
{
    colorPicker(name, colorConfig.color.data(), &colorConfig.color[3], &colorConfig.rainbow, &colorConfig.rainbowSpeed, &colorConfig.enabled, &colorConfig.thickness);
}

void ImGuiCustom::colorPicker(const char* name, ColorToggleThicknessRounding& colorConfig) noexcept
{
    colorPicker(name, colorConfig.color.data(), &colorConfig.color[3], &colorConfig.rainbow, &colorConfig.rainbowSpeed, &colorConfig.enabled, &colorConfig.thickness, &colorConfig.rounding);
}