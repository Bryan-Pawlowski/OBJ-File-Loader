using Veldrid;
using Veldrid.Sdl2;
using Veldrid.StartupUtilities;
namespace Net_Obj.GfxPrograms.MainWindow
{
    public class MainWindow_Base : IGfxProgram
    { 
        private Sdl2Window _window;
        private GraphicsDevice _device;
        
        /// <summary>
        /// Constructor for our main window.
        /// </summary>
        /// <param name="x">starting screen-space x position for the window</param>
        /// <param name="y">starting screen-space y position for the window</param>
        /// <param name="height">window width</param>
        /// <param name="width">window height</param>
        /// <param name="desc"></param>
        public MainWindow_Base(int x, int y, int height, int width, string desc)
        {
            WindowCreateInfo winCI = new WindowCreateInfo()
            {
                X = x,
                Y = y,
                WindowHeight = height,
                WindowWidth = width,
                WindowTitle = desc
            };

            _window = VeldridStartup.CreateWindow(ref winCI);

            GraphicsDeviceOptions options = new GraphicsDeviceOptions()
            {
                PreferStandardClipSpaceYDirection = true,
                PreferDepthRangeZeroToOne = true
            };
            _device = VeldridStartup.CreateGraphicsDevice(_window);
        }

        /// <summary>
        /// Implementation of <see cref="IGfxProgram.GetGfxDevice"/>
        /// </summary>
        public GraphicsDevice GetGfxDevice()
        {
            return _device;
        }

        /// <summary>
        /// Implementation of <see cref="IGfxProgram.GfxWindowExists"/>
        /// </summary>
        public bool GfxWindowExists()
        {
            return _window?.Exists ?? false;
        }

        public void Present()
        {
            _window.PumpEvents();
        }
        
    }
}