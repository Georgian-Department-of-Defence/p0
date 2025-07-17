#include "WidgetsScene.h"
#include "raygui.h"
#include "Widgets.h"

static Button button;

struct ClickTestData
{
    int number;
};

static void OnClickTest(void* data)
{
    ClickTestData& click_data = *(ClickTestData*)data;
    TraceLog(LOG_INFO, "Automatic button pressed. Data: %i", click_data.number);
}

static void RayguiExample();

void WidgetsScene::OnLoad(Game& game)
{
    button.rec = RecFromCenter(GetScreenWidth() / 2, GetScreenHeight() / 2, 80, 40);
    button.color_out = RED;
    button.color_in = ORANGE;
    button.color_click = GOLD;

    static ClickTestData click_test_data;
    click_test_data.number = 420;

    button.on_click = OnClickTest;
    button.on_click_data = &click_test_data;
}

void WidgetsScene::OnUnload(Game& game)
{
}

void WidgetsScene::OnStart(Game& game)
{
}

void WidgetsScene::OnStop(Game& game)
{
}

void WidgetsScene::OnUpdate(Game& game)
{
    UpdateButton(button, GetMousePosition());
}

void WidgetsScene::OnDraw(Game& game)
{
}

void WidgetsScene::OnDrawDebug(Game& game)
{
}

// The manual button has less setup, and not casting void* is nice, but its technically incorrect since it updates within draw
// Let's try and use automatic buttons where possible for clear separation between update vs draw
void WidgetsScene::OnDrawGui(Game& game)
{
    // Manual button:
    Rectangle btnTest = RecFromCenter(GetScreenWidth() / 2, GetScreenHeight() / 4, 80, 40);
    bool collision = CheckCollisionPointRec(GetMousePosition(), btnTest);
    DrawRectangleRec(btnTest, collision ? RED : GREEN);
    if (collision && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        TraceLog(LOG_INFO, "Manual button pressed. Data: %i", 69);
    }

    // Automatic button:
    DrawButton(button);

    // raygui is unsuitable for gear stat pannels since it can't draw textures
    // raygui is also "incorrect" according to my rules since it also does collision checks within draw...
    // raygui will only be applicable for developer tools ie map/particle/any editor
    RayguiExample();
}

// Is it less long-term effort to design gear stat pannels in-engine, or continue to represent stat pannels as textures and author them externally??
// (Don't worry about this for now. We need a system that selects loadouts before worrying about aesthetics and ease of use).
void RayguiExample()
{
    static bool showMessageBox = false;

    // May optionally include icons. I doubt we'll use heavily stylized GUI
    //int clicked = GuiButton({ 24, 24, 120, 30 }, GuiIconText(ICON_INFO, "Show Message"));
    //int clicked = GuiButton({ 24, 24, 120, 30 }, "#191#Show Message"); // Can also wrap icon id in #
    int clicked = GuiButton({ 24, 24, 120, 30 }, "Show Message");
    if (clicked)
        showMessageBox = true;

    if (showMessageBox)
    {
        int result = GuiMessageBox({ 85, 70, 250, 100 },
            "Message Box Title", "Do you like this message box?", "Yes;No");

        if (result >= 0)
        {
            showMessageBox = false;
            switch (result)
            {
            case 0:
                TraceLog(LOG_INFO, "X was clicked");
                break;

            case 1:
                TraceLog(LOG_INFO, "Yes was clicked");
                break;

            case 2:
                TraceLog(LOG_INFO, "No was clicked");
                break;
            }
        }
    }
}
