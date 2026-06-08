extends CanvasLayer

var hero = null
var game_over_started = false

func _ready():
	hide() # Na starcie gry ekran musi być ukryty
	hero = get_tree().current_scene.get_node_or_null("Hero")

func _process(delta):
	# Sprawdzamy, czy bohater istnieje i czy jeszcze nie zaczęliśmy procedury końca gry
	if is_instance_valid(hero) and not game_over_started:
		if hero.get_hp() <= 0:
			game_over_started = true
			_show_delayed_menu()

func _show_delayed_menu():
	# Czekamy 2 sekundy (czas na animację śmierci)
	await get_tree().create_timer(2.0).timeout
	show()

func _on_retry_button_pressed():
	get_tree().reload_current_scene()

func _on_menu_button_pressed():
	get_tree().change_scene_to_file("res://main_menu.tscn")
