#pragma once
#include "../gui/ColorSelectorGUI.hpp"
#include "GPUFractal.hpp"
#include "SDL_API.h"
#include "../FractalPanel.hpp"

namespace gan {
    /**
     * The @code FractalViewer@endcode provides an easy interface for interacting with fractals. They contain a
     * @code std::unique_ptr<Fractal>@endcode object inside, and directly use an ImGui Interface to interface with the fractal.
     *
     */
    class GPUFractalPanel final : public FractalPanel {
    public:
        // >>>>>>>>>>>>>>>>>>>>>>>>
        // Constructors/Destructors
        // >>>>>>>>>>>>>>>>>>>>>>>>
        /** Creates a fractal panel without a fractal **/
        GPUFractalPanel();
        ~GPUFractalPanel() override = default;

        /** Cannot copy a fractal panel */
        GPUFractalPanel(const GPUFractalPanel& other) = delete;
        GPUFractalPanel& operator=(const GPUFractalPanel& other) = delete;

        /** Default moveable */
        GPUFractalPanel(GPUFractalPanel&& other) = default;
        GPUFractalPanel& operator=(GPUFractalPanel&& other) = default;


        // >>>>>>>>>>>>>>>>>>>>>>>>
        // Member functions
        // >>>>>>>>>>>>>>>>>>>>>>>>
        /** Resizes the fractal panel to the given size and position.
         * @param window Window to draw to.
         * @param pos The position of the view in normalized coordinates (0.0 to 1.0)
         * @param size The size of the view in normalized coordinates (0.0 to 1.0) */
        void reframe(const Window& window, vec2 pos, vec2 size) override;

        void onMouseDown(const Window&, const Mouse&) override;
        void onMouseMotion(const Window&, const Mouse&) override;
        void onMouseWheel(const Window&, const Mouse&) override;
        void onMouseRelease(const Window&, const Mouse&) override;
        void onMouseLeave(const Window&, const Mouse&) override;
        void updateCursorPos(vec2 pos) override;

        /** Sets the starting position */
        void setCenterPos(vec2 startPos) const;

        /** Imbues a fractal in the window panel while taking ownership of the provided fractal.
         * @param id The fractal this panel will take ownership of.
         * @return True if the embed was successful. False if there was a compilation error.*/
        bool embed(size_t id) override;


        /** Begins the ImGui Display for this FractalViewer */
        void imguiBegin(const Window& window) override;
        /** Displays the Main ImGui interface for each individual fractal */
        void imguiBody(const Window& window) override ;
        /** Ends the ImGui instance **/
        void imguiEnd() override;

        /** Draws the fractal panel to the provided window. */
        void draw(const Window& window) override;

        // ~~~~~~~~~~~ Utilities ~~~~~~~~~~~~~~
        /** Checks whether the given normalized position is contained within the window **/
        [[nodiscard]] bool containedWithin(vec2 m_pos) const override;
        /** Return the position in fractal coordinates. Input should be from (0.0 to 1.0) */
        [[nodiscard]] vec2 normalizeToFractalPos(const Window& window, vec2 pos) const override;

    private:
        VBuffer<CVertex> borderVB{};        ///< Vertex buffer that displays the border around the fractal.
        std::string panelID;                ///< ID of the panel, so there is not ImGui name-clashes.
        vec2 imguiWindowSize{};
        bool clickedOn = false;

        void moveFractal(const Window& window, const Mouse& m);
        void zoom(const Window& window, const Mouse& mouse);

        ColorSelectorGUI color_selector;

        // ~~~~~~~~~~~~~~~~~~~~~~~~~
        std::unique_ptr<GPUFractal> frac;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~

        /// Generates the internal VertexBuffer object for the border.
        void genBorder(const Window& window, vec2 pos, vec2 size, vec4 color, float thickness);
        /// Displays the correct type of a special uniform in the ImGui renderer.
        void displayProperty(size_t id) const;
        /** Returns a string detailing the health of the Fractal Panel **/
        std::string checkHealth();
    };
}
