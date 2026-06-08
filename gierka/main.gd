extends Node2D

func _ready():
	# Sprawdzamy flagę z Menu
	if Global.load_from_save == true:
		await get_tree().process_frame
		_load_game_data()
		Global.load_from_save = false

# Funkcja wczytywania (odkodowuje JSON)
# Funkcja wczytywania (odkodowuje JSON)
func _load_game_data():
	var save_mgr = SaveManager.new()
	var json_string = save_mgr.load_game() # Pobieramy tekst z C++
	
	if json_string == "":
		print("Brak pliku save.txt")
		return
		
	# Rozpakowujemy tekst z powrotem do słownika (Dictionary)
	var json = JSON.new()
	var error = json.parse(json_string)
	
	if error == OK:
		var parsed_data = json.data # TO jest nasz słownik!
		var hero = get_node_or_null("Hero")
		
		if is_instance_valid(hero):
			# 1. Odczyt pozycji (Vector2)
			if parsed_data.has("x") and parsed_data.has("y"):
				hero.global_position = Vector2(parsed_data["x"], parsed_data["y"])
				
			# 2. Odczyt HP
			if parsed_data.has("hp") and hero.has_method("set_hp"):
				hero.set_hp(parsed_data["hp"])
				
			# 3. Odczyt ekwipunku
			if parsed_data.has("inventory") and hero.has_method("clear_inventory"):
				hero.clear_inventory()
				var saved_inv = parsed_data["inventory"]
				for item_name in saved_inv:
					if item_name != "Miecz":
						hero.add_to_inventory(item_name, saved_inv[item_name])
						
			# 4. ODCZYT I NISZCZENIE OBIEKTÓW (TEGO CI BRAKOWAŁO!)
			if parsed_data.has("destroyed"):
				Global.destroyed_objects = parsed_data["destroyed"]
				
				for obj_name in Global.destroyed_objects:
					var obj = find_child(obj_name, true, false) 
					if is_instance_valid(obj):
						obj.queue_free()
						
		print("Wczytano zapis z JSON pomyślnie!")
	else:
		print("Błąd odczytu pliku JSON!")

# Zapisywanie za pomocą klawisza ESC w trakcie gry (Zmienia w JSON)
func _input(event):
	if event.is_action_pressed("ui_cancel"): # Klawisz ESC
		_save_game_data()

func _save_game_data():
	var hero = get_node_or_null("Hero")
	if not is_instance_valid(hero): return

	# Budujemy paczkę danych do zapisania
	var save_dict = {
		"hp": hero.get_hp(),
		"x": hero.global_position.x,
		"y": hero.global_position.y,
		"destroyed": Global.destroyed_objects # <--- BRAKOWAŁO TEGO!
	}
	
	# Dodajemy ekwipunek
	if hero.has_method("get_inventory"):
		save_dict["inventory"] = hero.get_inventory()
		
	# Zapisujemy do JSON
	var json_string = JSON.stringify(save_dict)
	var save_mgr = SaveManager.new()
	save_mgr.save_game(json_string)
	
	print("Gra zapisana pod ESC (JSON)! Lista usuniętych obiektów: ", Global.destroyed_objects)
