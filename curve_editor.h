#include "imgui.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>	// I used glm for some stuff, sorry :(

enum class CurvePointType : uint8_t
{
	Linear,	// linear interp
	Constant,	// no interp
	SplitTangents,	// 2 handle free tangents
	Aligned,		// free tangents but they are kept aligned
};

class CurvePoint
{
public:

	float value = 0.0;
	float time = 0.0;
	glm::vec2 tangent0=glm::vec2(-1,0);
	glm::vec2 tangent1=glm::vec2(1,0);
	CurvePointType type = CurvePointType::Linear;
};

class Curve
{
public:
	std::vector<CurvePoint> points;
};

struct Color32
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0xff;

	uint32_t to_uint() const {
		return *(uint32_t*)this;
	}
};

struct EditingCurve
{
	Curve thecurve;
	std::string name = "";
	bool visible = true;
	Color32 color = { 255,0,255 };

	uint32_t curve_id = 0;
};
class CurveEditorImgui
{
public:
	CurveEditorImgui() {}

	// call this function to draw+update imgui interface
	void draw();
	
	// Max value on the X axis (min X axis is always 0)
	float max_x_value = 35.0;

	// Max/min values on the Y axis
	float min_y_value = 0.0;
	float max_y_value = 1.0;

	// current position of the scrubber, in X units
	float current_time = 0.0;

	// grid snap settings
	bool snap_scrubber_to_grid = false;
	bool enable_grid_snapping_x = true;
	bool enable_grid_snapping_y = true;
	ImVec2 grid_snap_size = ImVec2(1, 0.1);

	// retrieve any edited curves with this
	const std::vector<EditingCurve>& get_curve_array() const { return curves; }
	// add curves manuallly with this
	void add_curve(EditingCurve curve) {
		curve.curve_id = current_id_value++;
		curves.push_back(curve);
	}
	void clear_all() {
		curves.clear();
		set_selected_curve(-1);
		current_id_value = 0;
		dragging_point = false;
		dragged_point_index = -1;
		dragged_point_type = 0;
		point_index_for_popup = -1;
	}
private:
	void draw_editor_space();

	std::vector<EditingCurve> curves;

	// Scale is what gets changed with zooming, modifies base_scale
	ImVec2 scale = ImVec2(1, 1);
	// Base scale is constant, defines a screenspace -> grid unit factor
	const ImVec2 base_scale =ImVec2(1.0/32,-1.0/30);
	// Offset, defined in grid space
	ImVec2 grid_offset=ImVec2(0,0);

	// used internally for convenience
	ImVec2 BASE_SCREENPOS;	// base screenpos corner of graph content region
	ImVec2 WINDOW_SIZE;	// not actually the window size :P, its the content region that the graph draws into in the imgui table

	// state stuff
	int selected_curve = -1;
	// these are all indexing into selected_curve
	bool dragging_point = false;
	int dragged_point_index = -1;
	int dragged_point_type = 0;   // 0 = point, 1=tangent0,2=tangent1
	int point_index_for_popup = -1;

	bool dragging_scrubber = false;

	ImVec2 clickpos{};

	bool started_pan = false;
	ImVec2 pan_start = {};

	// used to identify curves internally
	uint32_t current_id_value = 0;

	void set_selected_curve(int index) {
		if (selected_curve != index) {
			// remove any state from setting points
			dragging_point = false;
			dragged_point_index = -1;
			dragged_point_type = 0;
			point_index_for_popup = -1;

			selected_curve = index;
		}
	}
	ImVec2 grid_to_screenspace(ImVec2 grid) const;
	ImVec2 screenspace_to_grid(ImVec2 screen) const;

	void clamp_point_to_grid(ImVec2& gridspace) {
		if (enable_grid_snapping_x) {
			gridspace.x = std::round(gridspace.x / grid_snap_size.x) * grid_snap_size.x;
		}
		if (enable_grid_snapping_y) {
			gridspace.y = std::round(gridspace.y / grid_snap_size.y) * grid_snap_size.y;
		}
		if (gridspace.x < 0)
			gridspace.x = 0;
		if (gridspace.x > max_x_value)
			gridspace.x = max_x_value;
		if (gridspace.y < min_y_value) gridspace.y = min_y_value;
		if (gridspace.y > max_y_value) gridspace.y = max_y_value;
	}
};
