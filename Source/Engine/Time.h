#pragma once

#include <GLFW/glfw3.h>

namespace UI {
	class Time {
	public:

		static void Update() {
			float currentFrame = static_cast<float>(glfwGetTime());
			s_DeltaTime = currentFrame - s_LastFrame;
			s_LastFrame = currentFrame;


			s_FrameCount++;
			s_TimeAccumulator += s_DeltaTime;

			if (s_TimeAccumulator >= 0.25f) {
				s_FPS = static_cast<float>(s_FrameCount) / s_TimeAccumulator;
				s_FrameCount = 0;
				s_TimeAccumulator = 0.0f;
			}
		}

		static float GetTime() { return static_cast<float>(glfwGetTime()); }
		static float GetDeltaTime() { return s_DeltaTime; }
		static float GetFPS() { return s_FPS; }

	private:
		inline static float s_DeltaTime = 0.0f;
		inline static float s_LastFrame = 0.0f;
		inline static float s_FPS = 0.0f;

		inline static int s_FrameCount = 0;
		inline static float s_TimeAccumulator = 0.0f;
	};
}
