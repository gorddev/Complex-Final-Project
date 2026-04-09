#pragma once
#include "gui-classes/ColorSelectorGUI.hpp"
#include "Fractal.hpp"
#include "core/types/vec.hpp"
#include "core/types/opengl/VBuffer.hpp"
#include "core/types/opengl/Vertex.hpp"
#include "core/window/Window.hpp"

namespace gan {
    /**
     * The @code FractalViewer@endcode provides an easy interface for interacting with fractals. They contain a
     * @code std::unique_ptr<Fractal>@endcode object inside, and directly use an ImGui Interface to interface with the fractal.
     *
     */
    class FractalPanel {
        VBuffer<CVertex> borderVB{};  ///< Vertex buffer that displays the border around the fractal.
        std::string panelID;        ///< ID of the panel, so there is not ImGui name-clashes.
        bool interactedThisFrame = false; ///< True if the panel was interacted with this frame.
        bool autoResize = false;
        vec2 imguiWindowSize{};

        ColorSelectorGUI color_selector;

        // ~~~~~~~~~~~~~~~~~~~~~~~~~
        std::unique_ptr<Fractal> frac;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~

        /// Generates the internal VertexBuffer object for the border.
        void genBorder(const Window& window, vec2 pos, vec2 size, vec4 color, float thickness);

    public:

        /** Creates a fractal panel without a fractal **/
        explicit FractalPanel(std::unique_ptr<Fractal> fractal = nullptr);
        ~FractalPanel();

        FractalPanel(const FractalPanel& other) = delete;
        FractalPanel& operator=(const FractalPanel& other) = delete;

        FractalPanel(FractalPanel&& other) = default;
        FractalPanel& operator=(FractalPanel&& other) = default;

        /** Resizes the fractal panel to the given size and position.
         * @param pos The position of the view in normalized coordinates (0.0 to 1.0)
         * @param size The size of the view in normalized coordinates (0.0 to 1.0) */
        void reframe(const Window& window, vec2 pos, vec2 size);
        /** Resizes the fractal panel in case of a window resize */
        void reframe(const Window& window);
        /** Moves the starting position of the fractal by the delta (delta should be normalized 0.0 to 1.0)*/
        void moveFractal(vec2 delta) const;
        /** Gets the current scale of the fractal **/
        float getScale();
        /** Sets the scale of the fractal **/
        void setScale(float scale);
        /** Gets the start position **/
        vec2 getStartPos() const;
        /** Sets the starting position */
        void setStartPos(vec2 startPos);

        /** Imbues a fractal in the window panel while taking ownership of the provided fractal.
         * @param fractal The fractal this panel will take ownership of.
         * @return The old fractal this window owns, if it currently has one. Nullptr otherwise. */
        std::unique_ptr<Fractal> embed(std::unique_ptr<Fractal> fractal);

        /** Extracts the unique fractal from this panel */
        [[nodiscard]] std::unique_ptr<Fractal> extract();

        /** Begins the ImGui Display for this FractalViewer */
        void imguiBegin(const Window& window);
        /** Displays the Main ImGui interface for each individual fractal */
        void imguiBody(const Window& window);

        void displayProperty(size_t id) const;

        /** Ends the ImGui instance **/
        void imguiEnd();

        /** Draws the fractal panel to the provided window. */
        void draw(const Window& window, vec2 selectionPos);

        // ~~~~~~~~~~~ Utilities ~~~~~~~~~~~~~~
        /** Returns a string detailing the health of the Fractal Panel **/
        std::string checkHealth();
        /** Checks whether the given normalized position is contained within the window **/
        bool containedWithin(vec2 pos) const;

        gan::vec2 normalizeToPanelPos(vec2 pos, const Window& window);

        /** Return the position in fractal coordinates. Input should be from (0.0 to 1.0) */
        vec2 normalizeToFractalPos(vec2 pos, const Window& window) const;
    };
}
