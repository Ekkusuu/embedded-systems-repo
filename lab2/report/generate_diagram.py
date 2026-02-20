import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
diagram_path = ROOT / "diagram.json"
out_path = Path(__file__).resolve().parent / "electrical_diagram.svg"

def esc(text: str) -> str:
    return (
        text.replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace('"', "&quot;")
    )

with diagram_path.open("r", encoding="utf-8") as f:
    data = json.load(f)

parts = data.get("parts", [])
connections = data.get("connections", [])

part_size = {
    "wokwi-arduino-uno": (220, 300),
    "wokwi-lcd1602": (250, 90),
    "wokwi-membrane-keypad": (180, 180),
    "wokwi-led": (30, 30),
    "wokwi-resistor": (70, 20),
}

part_fill = {
    "wokwi-arduino-uno": "#dbeafe",
    "wokwi-lcd1602": "#dcfce7",
    "wokwi-membrane-keypad": "#fef3c7",
    "wokwi-led": "#fee2e2",
    "wokwi-resistor": "#f3f4f6",
}

# Compute bounds
min_x, min_y = 10**9, 10**9
max_x, max_y = -10**9, -10**9
for p in parts:
    x = float(p.get("left", 0))
    y = float(p.get("top", 0))
    w, h = part_size.get(p.get("type", ""), (120, 60))
    min_x = min(min_x, x)
    min_y = min(min_y, y)
    max_x = max(max_x, x + w)
    max_y = max(max_y, y + h)

margin = 50
shift_x = margin - min_x
shift_y = margin - min_y
width = int(max_x - min_x + margin * 2)
height = int(max_y - min_y + margin * 2)

part_centers = {}
part_rects = {}
for p in parts:
    pid = p["id"]
    ptype = p.get("type", "")
    w, h = part_size.get(ptype, (120, 60))
    x = float(p.get("left", 0)) + shift_x
    y = float(p.get("top", 0)) + shift_y
    part_rects[pid] = (x, y, w, h, ptype)
    part_centers[pid] = (x + w / 2, y + h / 2)

svg = []
svg.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">')
svg.append('<rect x="0" y="0" width="100%" height="100%" fill="#ffffff"/>')
svg.append('<text x="20" y="30" font-family="Arial" font-size="20" font-weight="bold">Lab 2 Electrical Diagram</text>')

# Draw parts
for pid, (x, y, w, h, ptype) in part_rects.items():
    fill = part_fill.get(ptype, "#f8fafc")
    svg.append(
        f'<rect x="{x:.1f}" y="{y:.1f}" width="{w}" height="{h}" rx="8" ry="8" fill="{fill}" stroke="#111827" stroke-width="1.5"/>'
    )
    svg.append(
        f'<text x="{x + 8:.1f}" y="{y + 18:.1f}" font-family="Arial" font-size="13" font-weight="bold">{esc(pid)}</text>'
    )
    svg.append(
        f'<text x="{x + 8:.1f}" y="{y + 34:.1f}" font-family="Arial" font-size="11" fill="#374151">{esc(ptype)}</text>'
    )

# Draw connections
for conn in connections:
    src = conn[0]
    dst = conn[1]
    color = conn[2] if len(conn) > 2 else "#6b7280"
    route = conn[3] if len(conn) > 3 and isinstance(conn[3], list) else []

    src_part = src.split(":", 1)[0]
    dst_part = dst.split(":", 1)[0]

    if src_part not in part_centers or dst_part not in part_centers:
        continue

    x, y = part_centers[src_part]
    path_points = [(x, y)]

    for step in route:
        if not isinstance(step, str) or len(step) < 2:
            continue
        axis = step[0].lower()
        try:
            val = float(step[1:])
        except ValueError:
            continue
        if axis == "h":
            x += val
        elif axis == "v":
            y += val
        path_points.append((x, y))

    dx, dy = part_centers[dst_part]
    path_points.append((dx, dy))

    d = []
    for idx, (px, py) in enumerate(path_points):
        cmd = "M" if idx == 0 else "L"
        d.append(f"{cmd} {px:.1f} {py:.1f}")

    svg.append(
        f'<path d="{" ".join(d)}" fill="none" stroke="{esc(color)}" stroke-width="2" opacity="0.95"/>'
    )

    mx = (path_points[0][0] + path_points[-1][0]) / 2
    my = (path_points[0][1] + path_points[-1][1]) / 2
    label = f"{src} → {dst}"
    svg.append(
        f'<text x="{mx + 4:.1f}" y="{my - 4:.1f}" font-family="Arial" font-size="10" fill="#111827">{esc(label)}</text>'
    )

svg.append("</svg>")
out_path.write_text("\n".join(svg), encoding="utf-8")
print(f"Created: {out_path}")
