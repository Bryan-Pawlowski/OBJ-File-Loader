using Veldrid;
using Veldrid.Sdl2;
using Veldrid.StartupUtilities;
namespace Net_Obj.GfxPrograms.MainWindow
{
    public class MainWindow_Tutorial : MainWindow_Base
    {
        public MainWindow_Tutorial(int x, int y, int height, int width, string desc) : base(x, y, height, width, desc)
        { }

        public MainWindow_Tutorial() : this(100, 100, 540, 960, "My Tutorial Window")
        { }
    }
}