using System.ComponentModel;
using System.Data.SqlTypes;
using Veldrid;
using Veldrid.Sdl2;
using Veldrid.StartupUtilities;

namespace Net_Obj.GfxPrograms.MainWindow
{
    public class MainWindow_ImGui : MainWindow_Base
    {
        public MainWindow_ImGui(int x, int y, int height, int width, string desc) : base(x, y, height, width, desc)
        { }
        
        public MainWindow_ImGui() : this(100, 100, 540, 960, "My Window")
        { }
    }
}