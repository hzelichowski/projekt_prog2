extends HBoxContainer

var item_icons = {
	"Miecz": "res://assets/all/fc155.png",
	"Magiczny Napoj": "res://assets/all/fc288.png"
}

var hero = null
var last_inv_state = "{}"
var current_keys = [] 

func _ready():
	hero = get_tree().current_scene.get_node("Hero")

func _process(delta):
	if is_instance_valid(hero):
		var inv = hero.get_inventory()
		
		if str(inv) != last_inv_state:
			last_inv_state = str(inv)
			
			for child in get_children():
				child.queue_free()
				
			current_keys = []
			var slot_index = 1 
			
			for key in inv:
				current_keys.append(key)
				var btn = Button.new()
				btn.custom_minimum_size = Vector2(80, 80)
				
				if item_icons.has(key):
					btn.icon = load(item_icons[key])
					btn.expand_icon = true
					btn.icon_alignment = HORIZONTAL_ALIGNMENT_CENTER
					btn.vertical_icon_alignment = VERTICAL_ALIGNMENT_TOP
				
				if key == "Miecz":
					btn.text = "[" + str(slot_index) + "]"
				else:
					btn.text = "[" + str(slot_index) + "] x" + str(inv[key])
				
				btn.pressed.connect(func(): use_slot(key))
				add_child(btn)
				slot_index += 1

func _input(event):
	if event is InputEventKey and event.pressed and not event.echo:
		if event.keycode >= KEY_1 and event.keycode <= KEY_9:
			var index = event.keycode - KEY_1 
			
			if index < current_keys.size():
				use_slot(current_keys[index])

func use_slot(item_name):
	if is_instance_valid(hero) and item_name != "Miecz":
		hero.use_item(item_name)
