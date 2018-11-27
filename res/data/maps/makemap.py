import json

OUTPUT_FILE = "map2.json"
MAP_WIDTH = 10
MAP_HEIGHT = 10
TILE_SHEET = "res/assets/tiles/tilesheet2.png"
TILE_WIDTH = 64
TILESHEET_WIDTH = 10
TILESHEET_HEIGHT = 10

if __name__ == "__main__":
    output_data = {}
    output_data["width"] = MAP_WIDTH
    output_data["height"] = MAP_HEIGHT
    output_data["tilesheet"] = TILE_SHEET
    output_data["tilewidth"] = TILE_WIDTH
    output_data["tilehsheet_width"] = TILESHEET_WIDTH
    output_data["tilesheet_height"] = TILESHEET_HEIGHT
    output_data["tilemap"] = []
    output_data["collisionmap"] = []
    for i in range(MAP_WIDTH * MAP_HEIGHT):
        output_data["tilemap"] += [0]
    for i in range(MAP_WIDTH * MAP_HEIGHT):
        output_data["collisionmap"] += [0]
    with open(OUTPUT_FILE, "w+") as file:
        file.write(json.dumps(output_data, indent=4))
