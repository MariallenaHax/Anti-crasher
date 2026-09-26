#pragma once
#define PI 3.14
class RenderUtil
{
public:
	inline static void RenderText(Vector2<float> pos, std::string* textStr, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float textSize = 1.4f, float alpha = 1.f, int index = 0.f, ImDrawList* d = ImGui::GetBackgroundDrawList()) {
		if (!ImGui::GetCurrentContext()) return;

		constexpr float baseFontSize = 18.f;
		float fontSize = textSize * baseFontSize;
		ImVec2 shadowsize = ImVec2(textSize * 1.5f, textSize * 1.5f);

		d->AddText(ImGui::GetFont(), fontSize, ImVec2(pos.x + shadowsize.x, pos.y + shadowsize.y), ImColor(color.Value.x * 0.2f, color.Value.y * 0.2f, color.Value.z * 0.2f, alpha * 0.7f), textStr->c_str());
		d->AddText(ImGui::GetFont(), fontSize, ImVec2(pos.x, pos.y), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), textStr->c_str());
	}

	template <typename T>
	static void DrawBlur(Vector4<T> pos, float strength, float rounding = 0.f) {
		if (!ImGui::GetCurrentContext()) return;
	}

#pragma region Rectangles
	static void DrawRectangle(Vector4<float> pos, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, float rounding = 0.f, const ImDrawFlags& flags = ImDrawFlags_None, ImDrawList* list = ImGui::GetBackgroundDrawList()) {
		if (!ImGui::GetCurrentContext()) return;

		list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), rounding, flags);
	}

	static void DrawCustomRoundRectangle(Vector4<float> pos, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, ImVec4 rounding = ImVec4(0.f, 0.f, 0.f, 0.f)) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddRectFilledCustomRadius(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), rounding.x, rounding.z, rounding.y, rounding.w);
	}

	static void DrawRectangleShadow(Vector4<float> pos, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, float thickness = 1.f, ImDrawFlags flags = ImDrawFlags_None, float rounding = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList()) {
		if (!ImGui::GetCurrentContext()) return;

		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness, offset, flags, rounding);
	}
	static void DrawGradientRectangle(Vector4<float> pos, const ImColor& firstColor, const ImColor& secondColor, float rounding = 0.f, float firstAlpha = 1.f, float secondAlpha = 1.f, ImDrawFlags flags = ImDrawFlags_None) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		int startBufferSize = list->VtxBuffer.Size;
		list->AddRectFilled(topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.x, firstColor.Value.x, firstAlpha), rounding, flags);
		int endBufferSize = list->VtxBuffer.Size;
		list->AddRectFilled(topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.x, firstColor.Value.x, firstAlpha), rounding, flags);
		int endBufferSize2 = list->VtxBuffer.Size;

		ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, startBufferSize, endBufferSize, topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, secondAlpha));
		ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, endBufferSize, endBufferSize2, topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, secondAlpha));
	}

	static void DrawGradientRectangleOutline(Vector4<float> pos, const ImColor& firstColor, const ImColor& secondColor, float rounding = 0.f, float firstAlpha = 1.f, float secondAlpha = 1.f, float outlineThickness = 1.0f, const ImColor& outlineColor = ImColor{ 0, 0, 0, 1 }) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		ImVec2 shrinkedTopLeft = ImVec2(pos.x + outlineThickness, pos.y + outlineThickness);
		ImVec2 shrinkedBottomRight = ImVec2(pos.z - outlineThickness, pos.w - outlineThickness);

		int startBufferSize = list->VtxBuffer.Size;
		list->AddRect(shrinkedTopLeft, shrinkedBottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), rounding, 0, outlineThickness);
		int endBufferSize = list->VtxBuffer.Size;

		ImGui::ShadeVertsLinearColorGradientKeepAlpha(list, startBufferSize, endBufferSize, topLeft, bottomRight, ImColor(firstColor.Value.x, firstColor.Value.y, firstColor.Value.z, firstAlpha), ImColor(secondColor.Value.x, secondColor.Value.y, secondColor.Value.z, secondAlpha));
	}
#pragma endregion

#pragma region Circles
	static void DrawCircleShadow(Vector2<float> pos, float rounding, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, float thickness = 1.f, ImDrawFlags flags = ImDrawFlags_RoundCornersAll, float segments = 12.f) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowCircle(ImVec2(pos.x, pos.y), rounding, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness, offset, flags, rounding);
	}

	static void DrawCircleOutline(Vector2<float> center, float radius, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, int segments = 15.f, float thickness = 1.f) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircle(ImVec2(center.x, center.y), radius, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), segments, thickness);
	}

	static void DrawCircle(Vector2<float> center, float radius, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, int segments = 15.f) {
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircleFilled(ImVec2(center.x, center.y), radius, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), segments);
	}
#pragma endregion

#pragma region Others
	static void DrawSquareShadow(Vector4<float> center, float size, const ImColor& color = ImColor(255.f, 255.f, 255.f, 255.f), float alpha = 1.f, float thickness = 1.f, ImDrawFlags flags = ImDrawFlags_None) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);

		// Define the four corners of the square
		ImVec2 points[4];
		points[0] = ImVec2(center.x - size / 2.f, center.y - size / 2.f);
		points[1] = ImVec2(center.x + size / 2.f, center.y - size / 2.f);
		points[2] = ImVec2(center.x + size / 2.f, center.y + size / 2.f);
		points[3] = ImVec2(center.x - size / 2.f, center.y + size / 2.f);

		list->AddShadowConvexPoly(points, 4, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha), thickness, offset, flags);
	}
#pragma endregion

	static inline Vector2<float> GetScreenSize() {
		if (!Address::getClientInstance()) return Vector2<float>(0.f, 0.f);

		auto gd = Address::getClientInstance()->getGuiData();
		return Vector2<float>(gd->getmcResolution().x, gd->getmcResolution().y);
	}

	static bool IsFullScreen() {
		return true;
	}

	static inline float GetTextHeightFromString(std::string* textStr, float textSize) {
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18.f, FLT_MAX, -1.f, textStr->c_str()).y;
	}

	static inline float GetTextWidth(std::string* textStr, float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18.f, FLT_MAX, -1.f, textStr->c_str()).x;
	}

	static inline float GetTextHeight(float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18.f, FLT_MAX, -1.f, "").y;
	}

	static inline Vector2<float> getMousePos() {
		ImVec2 ImGuiMousePos = ImGui::GetIO().MousePos;
		return Vector2<float>(ImGuiMousePos.x, ImGuiMousePos.y);
	}

	inline static bool isMouseOver(Vector4<float>(pos)) {
		Vector2<float> mousePos = getMousePos();
		return mousePos.x >= pos.x && mousePos.y >= pos.y && mousePos.x < pos.z && mousePos.y < pos.w;
	}

	static inline float GetDelta() {
		return ImGui::GetIO().DeltaTime;
	}

	static __attribute__((always_inline)) void drawLine(Vector2<float> start, Vector2<float> end, ImColor color, float lineWidth) {
		if (!ImGui::GetCurrentContext()) return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImColor(color.Value.x, color.Value.y, color.Value.y, color.Value.w), lineWidth);
	}

	static Vector2<float> WorldToScreen(const Vector3<float>& world) {  //
		if (ImGui::GetCurrentContext()) {
			Vector2<float> ret;
			Vector2<float> windowSize = Vector2<float>(GetScreenSize().x + (IsFullScreen() ? 0.f : 10.f), GetScreenSize().y - (IsFullScreen() ? -10 : 5));
			Address::getClientInstance()->WorldToScreen(world, ret);
			return ret;
		}
	}

	static void drawLine3D(const Vector3<float>& start, const Vector3<float>& end, ImColor color) {
		if (!ImGui::GetCurrentContext()) return;

		Vector2<float> startScreen = WorldToScreen({ start.x, start.y, start.z });
		Vector2<float> endScreen = WorldToScreen({ end.x, end.y, end.z });

		drawLine(startScreen, endScreen, color, 1.4);
	}

	static void drawRing3D(const Vector3<float>& center, float radius, int segments, ImColor color) {
		if (!ImGui::GetCurrentContext()) return;
		std::vector<Vector3<float>> points;
		for (int i = 0; i < segments; ++i) {
			float angle = static_cast<float>(i) / static_cast<float>(segments) * 2.0f * PI;
			float x = center.x + radius * std::cos(angle);
			float y = center.y;
			float z = center.z + radius * std::sin(angle);
			points.push_back(Vector3<float>(x, y, z));
		}

		int ind = 0;

		for (size_t i = 0; i < points.size() - 1; ++i) {
			//Vector2<float> Pos1;
			int colorIndex = ind * 7;
			Vector2 Pos1 = WorldToScreen(center);
			if (!Address::getClientInstance()->WorldToScreen(center, Pos1)) continue;
			drawLine3D(points[i], points[i + 1], color);
			++ind;
		}
		drawLine3D(points.back(), points.front(), color);
	}

	static __attribute__((always_inline)) void drawBox(Vector3<float> lower, Vector3<float> upper, ImColor color, ImColor lineColor, float lineWidth, bool fill, bool outline) {
		Vector3<float> diff;
		diff.x = upper.x - lower.x;
		diff.y = upper.y - lower.y;
		diff.z = upper.z - lower.z;

		//Vector3<float> diff = upper.submissive(lower);
		Vector3<float> vertices[8];
		vertices[0] = Vector3<float>(lower.x, lower.y, lower.z);
		vertices[1] = Vector3<float>(lower.x + diff.x, lower.y, lower.z);
		vertices[2] = Vector3<float>(lower.x, lower.y + diff.y, lower.z);
		vertices[3] = Vector3<float>(lower.x + diff.x, lower.y + diff.y, lower.z);
		vertices[4] = Vector3<float>(lower.x, lower.y, lower.z + diff.z);
		vertices[5] = Vector3<float>(lower.x + diff.x, lower.y, lower.z + diff.z);
		vertices[6] = Vector3<float>(lower.x, lower.y + diff.y, lower.z + diff.z);
		vertices[7] = Vector3<float>(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

		auto instance = Address::getClientInstance();

		const auto d = ImGui::GetBackgroundDrawList();

		if (fill) {
			// Convert the vertices to screen coordinates
			std::vector<Vector2<float>> screenCords;
			for (int i = 0; i < 8; i++) {
				Vector2<float> screen;
				if (instance->WorldToScreen(vertices[i], screen)) {
					screenCords.push_back(screen);
				}
			}

			// Return if there are less than four points to draw quads with
			if (screenCords.size() < 8) return;

			// Define the indices of the vertices to use for each quad face
			std::vector<std::tuple<int, int, int, int>> faces = {
				{0, 1, 3, 2},  // Bottom face
				{4, 5, 7, 6},  // Top face
				{0, 1, 5, 4},  // Front face
				{2, 3, 7, 6},  // Back face
				{1, 3, 7, 5},  // Right face
				{0, 2, 6, 4}   // Left face
			};

			// Draw the quads to fill the box
			for (auto face : faces) {
				ImVec2 posScreenCords = ImVec2(screenCords[std::get<0>(face)].x, screenCords[std::get<0>(face)].y);
				ImVec2 posScreenCords1 = ImVec2(screenCords[std::get<1>(face)].x, screenCords[std::get<1>(face)].y);
				ImVec2 posScreenCords2 = ImVec2(screenCords[std::get<2>(face)].x, screenCords[std::get<2>(face)].y);
				ImVec2 posScreenCords3 = ImVec2(screenCords[std::get<3>(face)].x, screenCords[std::get<3>(face)].y);
				//d->AddImageRounded((void*)ClientOld::RenderInfo::HeadTexture, ImVec2(posScreenCords, posScreenCords1), ImVec2(posScreenCords2, posScreenCords3), ImVec2(0, 0), ImVec2(1, 1), HeadColor, 14.f);
				d->AddQuadFilled(posScreenCords, posScreenCords1, posScreenCords2, posScreenCords3, ImColor(color.Value.x, color.Value.y, color.Value.y, color.Value.w));
				//drawlist->AddQuadFilled(screenCords[std::get<3>(face)].toImVec2(), screenCords[std::get<2>(face)].toImVec2(), screenCords[std::get<1>(face)].toImVec2(), screenCords[std::get<0>(face)].toImVec2(), color.toImColor());
			}
		}

		{
			// Convert the vertices to screen coordinates
			std::vector<std::tuple<int, Vector2<float>>> screenCords;
			for (int i = 0; i < 8; i++) {
				Vector2<float> screen;
				if (instance->WorldToScreen(vertices[i], screen)) {
					screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
				}
			}

			// Return if there are less than two points to draw lines between
			if (screenCords.size() < 2) return;

			switch (outline) {
			case false: {
				// Draw lines between all pairs of vertices
				for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
					auto from = *it;
					auto fromOrig = vertices[std::get<0>(from)];

					for (auto to : screenCords) {
						auto toOrig = vertices[std::get<0>(to)];

						// Determine if the line should be drawn based on the relative positions of the vertices
						bool shouldDraw = false;
						// X direction
						shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
						// Y direction
						shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
						// Z direction
						shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

						ImVec2 posForm = ImVec2(std::get<1>(from).x, std::get<1>(from).y);
						ImVec2 posTo = ImVec2(std::get<1>(to).x, std::get<1>(to).y);
						if (shouldDraw) d->AddLine(posForm, posTo, ImColor(lineColor.Value.x, lineColor.Value.y, lineColor.Value.y), lineWidth);
					}
				}
				return;
				break;
			}
			case true: {
				// Find start vertex
				auto it = screenCords.begin();
				std::tuple<int, Vector2<float>> start = *it;
				it++;
				for (; it != screenCords.end(); it++) {
					auto cur = *it;
					if (std::get<1>(cur).x < std::get<1>(start).x) {
						start = cur;
					}
				}

				// Follow outer line
				std::vector<int> indices;

				auto current = start;
				indices.push_back(std::get<0>(current));
				Vector2<float> lastDir(0, -1);
				do {
					float smallestAngle = PI * 2;
					Vector2<float> smallestDir;
					std::tuple<int, Vector2<float>> smallestE;
					auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
					for (auto cur : screenCords) {
						if (std::get<0>(current) == std::get<0>(cur))
							continue;

						// angle between vecs
						Vector2<float> dir = Vector2<float>(std::get<1>(cur)).submissive(std::get<1>(current));
						float angle = atan2(dir.y, dir.x) - lastDirAtan2;
						if (angle > PI) {
							angle -= 2 * PI;
						}
						else if (angle <= -PI) {
							angle += 2 * PI;
						}
						if (angle >= 0 && angle < smallestAngle) {
							smallestAngle = angle;
							smallestDir = dir;
							smallestE = cur;
						}
					}
					indices.push_back(std::get<0>(smallestE));
					lastDir = smallestDir;
					current = smallestE;
				} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

				// draw

				Vector2<float> lastVertex;
				bool hasLastVertex = false;
				for (auto& indice : indices) {
					Vector2<float> curVertex = std::get<1>(screenCords[indice]);
					if (!hasLastVertex) {
						hasLastVertex = true;
						lastVertex = curVertex;
						continue;
					}
					ImVec2 lastVertexPos = ImVec2(lastVertex.x, lastVertex.y);
					ImVec2 curVertexPos = ImVec2(curVertex.x, curVertex.y);
					d->AddLine(lastVertexPos, curVertexPos, ImColor(lineColor.Value.x, lineColor.Value.y, lineColor.Value.y, lineColor.Value.w), lineWidth);
					lastVertex = curVertex;
				}
				return;
				break;
			}
			}
		}
	}

	static __attribute__((always_inline)) void drawCorners(Vector3<float> lower, Vector3<float> upper, ImColor color, float lineWidth) {
		if (Address::getLocalPlayer() == nullptr) return;
		Vector3<float> worldPoints[8];
		worldPoints[0] = Vector3<float>(lower.x, lower.y, lower.z);
		worldPoints[1] = Vector3<float>(lower.x, lower.y, upper.z);
		worldPoints[2] = Vector3<float>(upper.x, lower.y, lower.z);
		worldPoints[3] = Vector3<float>(upper.x, lower.y, upper.z);
		worldPoints[4] = Vector3<float>(lower.x, upper.y, lower.z);
		worldPoints[5] = Vector3<float>(lower.x, upper.y, upper.z);
		worldPoints[6] = Vector3<float>(upper.x, upper.y, lower.z);
		worldPoints[7] = Vector3<float>(upper.x, upper.y, upper.z);

		std::vector<Vector2<float>> points;
		for (int i = 0; i < 8; i++) {
			Vector2<float> result;
			if (Address::getClientInstance()->WorldToScreen(worldPoints[i], result))
				points.emplace_back(result);
		}
		if (points.size() < 1) return;

		Vector4<float> resultRect = { points[0].x, points[0].y, points[0].x, points[0].y };
		for (const auto& point : points) {
			if (point.x < resultRect.x) resultRect.x = point.x;
			if (point.y < resultRect.y) resultRect.y = point.y;
			if (point.x > resultRect.z) resultRect.z = point.x;
			if (point.y > resultRect.w) resultRect.w = point.y;
		}

		float length = (resultRect.x - resultRect.z) / 4.f;

		// Top left
		drawLine(Vector2(resultRect.x, resultRect.y), Vector2(resultRect.x - length, resultRect.y), color, lineWidth);
		drawLine(Vector2(resultRect.x, resultRect.y), Vector2(resultRect.x, resultRect.y - length), color, lineWidth);

		// Top right
		drawLine(Vector2(resultRect.z, resultRect.y), Vector2(resultRect.z + length, resultRect.y), color, lineWidth);
		drawLine(Vector2(resultRect.z, resultRect.y), Vector2(resultRect.z, resultRect.y - length), color, lineWidth);

		// Bottom left
		drawLine(Vector2(resultRect.x, resultRect.w), Vector2(resultRect.x - length, resultRect.w), color, lineWidth);
		drawLine(Vector2(resultRect.x, resultRect.w), Vector2(resultRect.x, resultRect.w + length), color, lineWidth);

		// Bottom right
		drawLine(Vector2(resultRect.z, resultRect.w), Vector2(resultRect.z + length, resultRect.w), color, lineWidth);
		drawLine(Vector2(resultRect.z, resultRect.w), Vector2(resultRect.z, resultRect.w + length), color, lineWidth);
	}

	static void drawCircle3D(Vector3<float> const& center, float size, float thickness, ImColor const& color, float resolution) {
		Vector2<float> previousScreenPos;
		for (float angle = 0.0f; angle <= 360; angle += resolution) {
			float radians = angle * PI / 180;
			float x = center.x + std::sinf(radians) * size;
			float z = center.z + std::cosf(radians) * size;
			Vector3<float> worldPos = Vector3<float>(x, center.y, z);

			Vector2<float> screenPos;
			Address::getClientInstance()->WorldToScreen(worldPos, screenPos);

			drawLine(Vector2(previousScreenPos.x, previousScreenPos.y), screenPos, color, thickness);
			previousScreenPos = screenPos;
		}
	}
};

class ImScaleUtil {
public:
	ImScaleUtil() {
		scale_start_index = 0.f;
	}

	static void ImScaleStart()
	{
		scale_start_index = ImGui::GetBackgroundDrawList()->VtxBuffer.Size;
	}

	static inline int scale_start_index;

	static ImVec2 ImScaleCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

		const auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
		for (int i = scale_start_index; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	static void ImScaleEnd(float scaleX, float scaleY, ImVec2 center = ImScaleCenter())
	{
		auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;

		for (int i = scale_start_index; i < buf.Size; i++)
		{
			ImVec2 pos = ImVec2(buf[i].pos.x - center.x, buf[i].pos.y - center.y);
			pos.x *= scaleX;
			pos.y *= scaleY;
			buf[i].pos = ImVec2(pos.x + center.x, pos.y + center.y);
		}
	}
};

class ImRotateUtil {
public:
	ImRotateUtil() {
		rotationStartIndex = 0.f;
	}

	static void startRotation() {
		rotationStartIndex = ImGui::GetBackgroundDrawList()->VtxBuffer.Size;
	}

	static ImVec2 getRotationCenter() {
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

		const auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
		for (int i = rotationStartIndex; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	static void endRotation(float rad, ImVec2 center = getRotationCenter()) {
		rad += PI * 0.5f;
		float s = sin(rad), c = cos(rad);
		center = ImVec2(ImRotate(center, s, c).x - center.x, ImRotate(center, s, c).y - center.y);

		auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;

		for (int i = rotationStartIndex; i < buf.Size; i++)
			buf[i].pos = ImVec2(ImRotate(buf[i].pos, s, c).x - center.x, ImRotate(buf[i].pos, s, c).y - center.y);
	}

private:
	inline static int rotationStartIndex = 0;
};

class ClipUtil {
public:
	static void beginClipping(Vector4<float> rect) {
		ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(rect.x, rect.y), ImVec2(rect.z, rect.w), true);
	}

	static void restoreClipping() {
		ImGui::GetBackgroundDrawList()->PopClipRect();
	}
};
#pragma endregion