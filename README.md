**Requires: imgui, glm**

To use: create a CurveEditorImgui instance and call .draw() when updating your ImGui interfaces

### Controls: 

Hold middle mouse and drag to pan.

Scroll wheel to zoom the X-axis.

Scroll wheel plus ctrl to zoom the Y-axis.

Create curves by clicking 'Add row' in the sidebar, select the row by clicking on it. Right click the row in the sidebar to change the curve's name, change the curve's color, or delete the curve. The checkbox toggles the curves visibility.

*(Important!)* With a row **SELECTED** in the sidebar, right click on the graph area to add a point to it's curve.

Click and drag points with the left mouse button.

Right click on a point to open its options. You can change the interpolation type or delete the point. The interpolation types are linear,constant,aligned,and free tangents.

When a point is using aligned or free tangents, two tangent handles appear which you can drag like points to adjust the curve.

Change min/max values and grid snapping options under the 'Options..' menu.

The editor also provides a time marker that can be dragged, this has no effect on the editor and is for the user to use.



