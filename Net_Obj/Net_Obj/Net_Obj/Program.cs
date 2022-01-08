using System;
using System.Reflection.Metadata.Ecma335;
using Net_Obj.GfxPrograms;
using Net_Obj.GfxPrograms.MainWindow;
using Veldrid;
using Veldrid.Sdl2;
using Veldrid.StartupUtilities;

namespace Net_Obj
{
    /// <summary>
    /// This program is meant to have users point to a set of shaders to create a shader
    /// pipeline.
    ///
    /// This program is meant as an exercise to demonstrate graphics API usage in a readable way
    /// (hence the use of c#).
    /// </summary>
    class Program
    {

        
        private static IGfxProgram _mainProgram;

        /// <summary>
        /// static ref to the second swapchain we will be using for this program's second window.
        /// This window will be used to display our shader program.
        /// </summary>
        private static Swapchain _altSwapchain;
        
        /// <summary>
        ///  secondary window we will be using to display our shader program.
        /// </summary>
        private static Sdl2Window _altWindow;
        
        /// <summary>
        /// Main entrypoint into our program. This will be responsible for getting our boilerplate
        /// code set up, then run our different window "sub-programs" in a main loop.
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            BoilerplateStartup();
            MainLoop();
        }

        /// <summary>
        /// This is where our steady-state app loop lives. Without this, the app would render
        /// one frame (or none) and then destroy itself.
        /// </summary>
        static void MainLoop()
        {
            while (true)
            {
                //check if this window has been exited. 
                //if so, close out the app.
                if (_mainProgram.GfxWindowExists())
                {
                    _mainProgram.Present();
                    continue;
                }
                break;
            }
        }

        
        static void BoilerplateStartup()
        {
            _mainProgram = new MainWindow_Tutorial(100, 100, 540, 960, "Tutorial Window");
        }
    }
}