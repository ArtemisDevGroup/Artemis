#ifndef __ARTEMIS_IMGUI_WINDOWS_HXX__
#define __ARTEMIS_IMGUI_WINDOWS_HXX__

#include "Definitions.hxx"

#include "API/Logging.hxx"

#include <string_view>	// std::string_view
#include <type_traits>	// std::is_base_of_v

namespace Artemis {
	/// <summary>
	/// <para>Controls whether all ImGui windows should be hidden or shown.</para>
	/// <para>This bool has no effect on the individual visibility of each window instance.</para>
	/// </summary>
	ARTEMIS_FRAMEWORK extern bool HideAllWindows;

	/// <summary>
	/// The base class of all ImGui windows created via Artemis.
	/// </summary>
	class iwindow : public API::loggable {
		std::string_view _WindowName;

	protected:
		/// <summary>
		/// Contains all ImGui appends to the window instance.
		/// </summary>
		virtual void window() = 0;

	public:
		/// <summary>
		/// Defines whether to show the ImGui window or not.
		/// </summary>
		bool IsWindowVisible;

		/// <summary>
		/// Initializes a new window instance.
		/// </summary>
		/// <param name="_WindowName">- The name of the window.</param>
		ARTEMIS_FRAMEWORK iwindow(std::string_view&& _WindowName) noexcept;

		/// <summary>
		/// Presents the window. Is only intended to be called by the DirectX hook.
		/// </summary>
		ARTEMIS_FRAMEWORK void present() noexcept;

		/// <summary>
		/// Gets the name of the window.
		/// </summary>
		/// <returns>The window name.</returns>
		ARTEMIS_FRAMEWORK const std::string_view& name() const noexcept;
	};

	/// <summary>
	/// A type derived from the iwindow interface.
	/// </summary>
	template<class _Ty>
	concept derived_window_type = std::is_base_of_v<iwindow, _Ty>;

	/// <summary>
	/// Manages ImGui windows.
	/// </summary>
	class window_manager : public API::loggable{
		std::vector<iwindow*> _WindowInstances;

	public:
		ARTEMIS_FRAMEWORK window_manager() noexcept;

		window_manager(const window_manager&) = delete;
		ARTEMIS_FRAMEWORK window_manager(window_manager&&) noexcept;

		ARTEMIS_FRAMEWORK ~window_manager() noexcept;

		/// <summary>
		/// Registers a window instance to be presented to the screen.
		/// </summary>
		/// <param name="_WindowInstance">- A newly allocated pointer to the window instance.</param>
		/// <returns>The registered window instance pointer.</returns>
		/// <exception cref="argument_exception"/>
		ARTEMIS_FRAMEWORK iwindow* register_window(iwindow* _WindowInstance);

		/// <summary>
		/// Registers a window instance to be presented to the screen.
		/// </summary>
		/// <typeparam name="_WndTy">The derived window instance type.</typeparam>
		/// <typeparam name="_Types">The derived window instance constructor types.</typeparam>
		/// <param name="_Args">- Arguments to be passed to the derived window type's constructor.</param>
		/// <returns>The registered window instance pointer.</returns>
		/// <exception cref="argument_exception"/>
		template<derived_window_type _WndTy, typename... _Types>
		inline iwindow* register_window(_Types&&... _Args) {
			return this->register_window(new _WndTy(std::forward<_Types>(_Args)...));
		}

		/// <summary>
		/// Gets a pointer from a window instance from the window name.
		/// </summary>
		/// <param name="_WindowName">The window name. Note that the window name is case sensitive.</param>
		/// <returns>The found window instance pointer.</returns>
		/// <exception cref="argument_exception"/>
		ARTEMIS_FRAMEWORK iwindow* get_window(std::string_view&& _WindowName);
		
		/// <summary>
		/// Gets a pointer from a window instance from the window name.
		/// </summary>
		/// <typeparam name="_WndTy">The derived window type.</typeparam>
		/// <param name="_WindowName">The window name. Note that the window name is case sensitive.</param>
		/// <returns>The found window instance pointer.</returns>
		/// <exception cref="argument_exception"/>
		template<derived_window_type _WndTy>
		inline _WndTy* get_window(std::string_view&& _WindowName) {
			return (_WndTy*)this->get_window(std::forward<std::string_view>(_WindowName));
		}

		/// <summary>
		/// Removes a window from the stack of presentable windows.
		/// </summary>
		/// <param name="_WindowInstance">- A pointer to the window instance to remove.</param>
		/// <exception cref="argument_exception"/>
		ARTEMIS_FRAMEWORK void remove_window(iwindow* _WindowInstance);

		/// <summary>
		/// Removes a window from the stack of presentable windows.
		/// </summary>
		/// <param name="_WindowName">- The name of the window to remove.</param>
		/// <exception cref="argument_exception"/>
		ARTEMIS_FRAMEWORK void remove_window(std::string_view&& _WindowName);

		/// <summary>
		/// Presents all registered windows. Is only intended to be called by the DirectX hook.
		/// </summary>
		ARTEMIS_FRAMEWORK void present_all() noexcept;

		window_manager& operator=(const window_manager&) = delete;
		ARTEMIS_FRAMEWORK window_manager& operator=(window_manager&&) noexcept;
	};
}

#endif // !__ARTEMIS_IMGUI_WINDOWS_HXX__