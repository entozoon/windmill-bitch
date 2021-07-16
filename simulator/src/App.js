import { useState, useEffect } from "react";
import "./App.css";
function App() {
  const [rotationCourseTo, setRotationCourseTo] = useState(0);
  const [rotationCourse, setRotationCourse] = useState(0);
  const [rotationBearing, setRotationBearing] = useState(0);
  useEffect(() => {
    let bearing = rotationCourseTo - rotationCourse;
    // Keep it in the positive realm
    bearing = bearing < 0 ? 360 + bearing : bearing;
    setRotationBearing(bearing);
    console.log(rotationBearing);
  }, [rotationCourse, rotationCourseTo]);
  return (
    <div className="App">
      <div
        style={{
          position: "absolute",
          top: "50px",
          left: "300px",
        }}
      >
        <label>gps.courseTo 🧭</label>
        <br />
        <input
          {...{
            type: "range",
            min: 0,
            max: 360,
          }}
          onChange={(e) => {
            setRotationCourseTo(e.target.value);
          }}
        />
        <div
          className="line"
          style={{
            background: "black",
            transform: `rotate(${rotationCourseTo || 0}deg)`,
          }}
        />
      </div>
      <div
        style={{
          position: "absolute",
          top: "450px",
          left: "300px",
        }}
      >
        <label>gps.course 🚙</label>
        <br />
        <input
          {...{
            type: "range",
            min: 0,
            max: 360,
          }}
          onChange={(e) => {
            setRotationCourse(e.target.value);
          }}
        />
        <div
          className="line"
          style={{
            background: "blue",
            transform: `rotate(${rotationCourse || 0}deg)`,
          }}
        />
      </div>
      <div
        style={{
          position: "absolute",
          top: "300px",
          left: "800px",
        }}
      >
        <div
          className="line"
          style={{
            background: "red",
            transform: `rotate(${rotationBearing || 0}deg)`,
          }}
        />
      </div>
    </div>
  );
}
export default App;
