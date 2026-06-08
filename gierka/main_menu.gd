extends Control

func _ready():
	# Pobieramy Twoje 3 przyciski zgodnie z ich nazwami w drzewku
	var btn_play = $VBoxContainer/Button     # Nowa Gra
	var btn_load = $VBoxContainer/Button2    # Wczytaj Grę
	var btn_exit = $VBoxContainer/Button3    # Wyjście
	
	# Łączymy je z funkcjami
	btn_play.pressed.connect(_on_play_pressed)
	btn_load.pressed.connect(_on_load_pressed)
	btn_exit.pressed.connect(_on_exit_pressed)

func _on_play_pressed():
	# Startujemy od zera, więc musimy wyczyścić pamięć o starych wrogach!
	Global.load_from_save = false
	Global.destroyed_objects.clear() 
	
	# Włączamy planszę
	get_tree().change_scene_to_file("res://main.tscn")

func _on_load_pressed():
	# Ustawiamy flagę, że chcemy wczytać zapis
	Global.load_from_save = true
	
	# Włączamy planszę (To tutaj był błąd!)
	get_tree().change_scene_to_file("res://main.tscn")

func _on_exit_pressed():
	# Zamykamy grę
	get_tree().quit()
