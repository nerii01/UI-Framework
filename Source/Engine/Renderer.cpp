#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>    
#include "../Managers/ImageManager.h"

#ifdef DrawText
#undef DrawText
#endif

namespace UI {

		void Renderer::DrawRect(glm::vec2 position, glm::vec2 size, const Style& style, bool debug, const std::string& imagePath) {
			//Logger::Get().PrintToConsole("Draw Call", LogDebug);
			// Style data
			float scale = style.Scale.value();
			glm::vec4 backgroundColor = style.BackgroundColor.value();
			float noiseAmount = 1.0f;

			// Model
			glm::vec2 center = position + size * 0.5f;
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));     
			model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
			model = glm::translate(model, glm::vec3(-size * 0.5f, 0.0f));                  
			model = glm::scale(model, glm::vec3(size, 1.0f));

		

			// Uniforms
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, (int)ImageManager::Get().GetImage(imagePath));
			
			// Projection / Model / RenderMode
			shader.SetUniform("u_Image", 1);
			shader.SetUniform("u_Projection", m_Projection);
			shader.SetUniform("u_Model", model);
			shader.SetUniform("u_RenderMode", (int)RenderMode::Rect);

			// Position / Size
			shader.SetUniform("u_Widget.Position", position);
			shader.SetUniform("u_Widget.Size", size);
			shader.SetUniform("u_Widget.Scale", scale);

			// Background / Noise / Gradient
			shader.SetUniform("u_Rect.BackgroundColor", backgroundColor);
			shader.SetUniform("u_Noise.Amount", noiseAmount);

			// Border / Border radius
			shader.SetUniform("u_Rect.BackgroundColor", backgroundColor);
			shader.SetUniform("u_Noise.Amount", noiseAmount);

			//glm::vec2 cursorPos = InputManager::Get().GetCursorPosition(); ------ MOOVED
			shader.SetUniform("u_Rect.BorderWidth", style.Border.value().Width);
			shader.SetUniform("u_Rect.BorderColor", style.Border.value().Color);
			shader.SetUniform("u_Rect.BorderRadius", style.Border.value().Radius);
			shader.SetUniform("u_Rect.HasImage", imagePath != "");
			//shader.SetUniform("u_Rect.BorderHighlight", cursorPos);


			// Draw Gradient
			if (style.Gradient.has_value()) {
				GradientData gradient = style.Gradient.value();
				shader.SetUniform("u_Gradient.Color", gradient.Color);
				shader.SetUniform("u_Gradient.Position", gradient.Position);
				shader.SetUniform("u_Gradient.Radius", gradient.Radius);
				shader.SetUniform("u_Gradient.Hardness", gradient.Hardness);
			}
			// No Gradient
			else {
				shader.SetUniform("u_Gradient.Color", glm::vec4(0.0f));
			}

			// Draw call
			glBindVertexArray(GetCurrentVAO());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		void Renderer::DrawText(const std::string& fontName, const std::string& text, const glm::vec2& position, const glm::vec4& color, float fontSize, float availableWidth, WrapMode wrapmode, bool debug) {
			auto font = FontManager::Get().GetFont(fontName);
			if (font == nullptr) return;
			if (font->glyphs.empty()) {
				Logger::Get().PrintToConsole("Loaded font {} has 0 glyphs", LogType::Error, fontName);
				return;
			}

			fontSize = std::clamp(fontSize, 8.0f, 96.0f);
			float scale = (fontSize / font->fontSize);
			float lineHeight = std::abs(font->ascent - font->descent) * scale;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, font->atlasTexture);

			shader.SetUniform("u_FontAtlas", 0);
			float ascent = std::round(font->ascent * scale);

			float cursorX = position.x;
			float cursorY = std::round(position.y) + (ascent * 0.85f);

			std::vector<std::string> fontWrapped = FontManager::Get().WrapText(text, fontName, fontSize, availableWidth, wrapmode);

			for (const auto& line : fontWrapped) {
				for (const auto& character : line) {
					auto it = font->glyphs.find(character);
					if (it == font->glyphs.end()) continue;
					
					const auto& glyph = it->second;
					
					float x0 = cursorX + glyph.xoff * scale;
					float y0 = cursorY + glyph.yoff * scale;

					float x1 = x0 + glyph.width * scale;
					float y1 = y0 + glyph.height * scale;

					float drawX = std::round(x0);
					float drawY = std::round(y0);
					float drawW = std::round(x1) - drawX;
					float drawH = std::round(y1) - drawY;

					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(drawX, drawY, 0.0f));
					model = glm::scale(model, glm::vec3(drawW, drawH, 1.0f));

					shader.SetUniform("u_RenderMode", (int)RenderMode::Text);

					float u0 = (float)glyph.atlasX / font->atlasWidth;
					float v0 = (float)glyph.atlasY / font->atlasHeight;

					float u1 = (float)(glyph.atlasX + glyph.width) / font->atlasWidth;
					float v1 = (float)(glyph.atlasY + glyph.height) / font->atlasHeight;

					// Projection / Model / UVRect
					shader.SetUniform("u_Projection", m_Projection);
					shader.SetUniform("u_Model", model);
					shader.SetUniform("u_UVRect", glm::vec4(u0, v0, u1, v1));
					shader.SetUniform("u_Text.TextColor", color);

					glBindVertexArray(GetCurrentVAO());
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
					glBindVertexArray(0);

					cursorX += glyph.xAdvance * scale;
				}
				cursorX = position.x;
				cursorY += lineHeight;
			}
		}

		GLuint Renderer::GetCurrentVAO() {
			GLFWwindow* currentContext = glfwGetCurrentContext();
			if (!currentContext) return 0;

			auto it = m_VAOs.find(currentContext);
			if (it != m_VAOs.end()) {
				return it->second;
			}

			GLuint vao = 0;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// BARDZO WAŻNE: Podpinamy JUŻ ISTNIEJĄCE, współdzielone VBO i IBO pod nowy VAO
			glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIBO);

			// Ustawienia atrybutów dla tego konkretnego VAO
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

			// Odpinamy VAO dla bezpieczeństwa stanu
			glBindVertexArray(0);

			m_VAOs[currentContext] = vao;
			return vao;
		}

		void Renderer::DrawImage(glm::vec2 position, glm::vec2 size, const std::string& imagePath) {
				// Model
				glm::vec2 center = position + size * 0.5f;
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f));
				model = glm::translate(model, glm::vec3(-size * 0.5f, 0.0f));
				model = glm::scale(model, glm::vec3(size, 1.0f));


				// Uniforms
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, ImageManager::Get().GetImage(imagePath));
				shader.SetUniform("u_Image", 1);

				// Projection / Model / RenderMode
				shader.SetUniform("u_Projection", m_Projection);
				shader.SetUniform("u_Model", model);
				shader.SetUniform("u_RenderMode", (int)RenderMode::Rect);

				// Position / Size
				shader.SetUniform("u_Widget.Position", position);
				shader.SetUniform("u_Widget.Size", size);

				// Draw call
				glBindVertexArray(GetCurrentVAO());
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				glBindVertexArray(0);

		};

		void Renderer::BeginFrame(int screenWidth, int screenHeight) {
			// Projection
			m_Projection = glm::ortho(
				0.0f, static_cast<float>(screenWidth),
				static_cast<float>(screenHeight), 0.0f,
				-1.0f, 1.0f
			);

			// Shader activation and global shader variables
			//glm::vec2 cursorPos = InputManager::Get().GetCursorPosition();
			shader.Use();
			//shader.SetUniform("u_Global.CursorPosition", cursorPos);
			shader.SetUniform("u_Global.Time", static_cast<float>(glfwGetTime()));
		}

		void Renderer::InitQuad() {
			// Generate VBO VAO IBO
			// Dzięki flagi `share` w glfwCreateWindow będą widoczne we wszystkich kontekstach.
			glGenBuffers(1, &m_QuadVBO);
			glGenBuffers(1, &m_QuadIBO);

			glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// Shader locations
			std::vector<std::string> uniforms = {
				// Basic Transforms
				"u_Projection",
				"u_Model",
				"u_UVRect",
				"u_RenderMode",

				// Global
				"u_Global.Time",
				"u_Global.CursorPosition",

				// Widget
				"u_Widget.Position",
				"u_Widget.Size",

				// Rect
				"u_Rect.BackgroundColor",

				// Text
				"u_Text.TextColor",
				"u_Text.FontAtlas",

				// Gradient
				"u_Gradient.Color",
				"u_Gradient.Position",
				"u_Gradient.Hardness",
				"u_Gradient.Radius",
			};

			shader.Use();
			shader.SetUniforms(uniforms);
		}

}