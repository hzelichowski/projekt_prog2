extends Control

@onready var save_manager = $SaveManager

func _ready():
	# Na samym starcie chowamy menu, żeby nie zasłaniało gry
	hide() 
	
	# Podpinamy przyciski
	$VBoxContainer/Button.pressed.connect(_on_save_pressed)
	$VBoxContainer/Button2.pressed.connect(_on_load_pressed)
	$VBoxContainer/Button3.pressed.connect(_on_exit_pressed)

func _input(event):
	# ui_cancel to domyślnie ESC. Sprawdzamy, czy został wciśnięty
	if event.is_action_pressed("ui_cancel"):
		toggle_pause()

func toggle_pause():
	# Odwracamy obecny stan (jeśli pauza była false, to teraz jest true i na odwrót)
	var is_paused = not get_tree().paused
	get_tree().paused = is_paused
	visible = is_paused # Pokazujemy lub chowamy menu

func _on_save_pressed():
	# 1. Szukamy naszego bohatera w głównej scenie
	var hero = get_tree().current_scene.get_node_or_null("Hero")
	
	if not is_instance_valid(hero):
		print("Błąd zapisu: Nie znaleziono Bohatera!")
		return

	# 2. Budujemy słownik ze wszystkimi statystykami
	var save_dict = {
		"hp": hero.get_hp(),
		"x": hero.global_position.x,
		"y": hero.global_position.y,
		"destroyed": Global.destroyed_objects # Lista zabitych wrogów i zebranych łupów
	}
	
	# 3. Jeśli dodałeś funkcję ekwipunku, dorzucamy go do paczki
	if hero.has_method("get_inventory"):
		save_dict["inventory"] = hero.get_inventory()
		
	# 4. Pakujemy cały słownik w jeden długi tekst (JSON) - TO JEST TEN 1 ARGUMENT!
	var json_string = JSON.stringify(save_dict)
	
	# 5. Wysyłamy do C++
	var save_mgr = SaveManager.new()
	save_mgr.save_game(json_string)
	
	print("Gra zapisana pomyślnie z Menu Pauzy!")

func _on_load_pressed():
	# 1. Zdejmujemy pauzę i wracamy do gry (aby wszystko znów działało)
	toggle_pause() 
	
	# 2. Ustawiamy naszą globalną flagę, żeby główny skrypt wiedział co robić
	Global.load_from_save = true
	
	# 3. Przeładowujemy całą mapę! (To gwarantuje, że starzy wrogowie znikną, a martwi zresetują się przed załadowaniem zapisu)
	get_tree().reload_current_scene()

func _on_exit_pressed():
	# BARDZO WAŻNE: Zdejmujemy pauzę przed wyjściem! 
	# Inaczej Main Menu też byłoby zamrożone.
	get_tree().paused = false 
	get_tree().change_scene_to_file("res://main_menu.tscn")
