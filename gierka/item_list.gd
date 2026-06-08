extends ItemList

var hero = null
var current_keys = []
var last_inv_state = "{}"

func _ready():
	hero = get_tree().current_scene.get_node("Hero")

func _process(delta):
	if is_instance_valid(hero):
		var inv = hero.get_inventory()
		# Odświeżamy listę tylko wtedy, gdy zawartość plecaka uległa zmianie
		if str(inv) != last_inv_state:
			last_inv_state = str(inv)
			clear()
			current_keys = []
			
			for key in inv:
				add_item(key + " (x" + str(inv[key]) + ")")
				current_keys.append(key)

func _on_item_activated(index):
	# Wywołuje się, gdy dwukrotnie klikniemy przedmiot na liście
	if is_instance_valid(hero):
		var item_name = current_keys[index]
		hero.use_item(item_name)
