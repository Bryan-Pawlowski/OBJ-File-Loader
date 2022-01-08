using Veldrid;

namespace Net_Obj.GfxPrograms
{
    public interface IGfxProgram
    {
        /// <summary>
        /// present contents of program to its assigned swapchain.
        /// </summary>
        public void Present();

        /// <summary>
        /// Checks to see if a graphics program's assigned window exists.
        /// </summary>
        /// <returns>
        /// <b>TRUE</b> - iff the graphics program's assigned window is open (exists)
        /// <b>FALSE</b> - if the graphics program's assigned window does not exist (is closed)
        /// </returns>
        public bool GfxWindowExists();

        /// <summary>
        /// Gets the graphics device created from this program (if this is the first gfx window program
        /// instanced for this application, this method is needed for retrieving the device for the rest of our window
        /// programs.
        /// </summary>
        /// <returns>
        /// Reference to the program's <see cref="GraphicsDevice"/>.
        /// </returns>
        public GraphicsDevice GetGfxDevice();
    }
}