extends ProgressBar

var hero = null

func _ready():
	# Szukamy naszego gracza w głównej scenie
	hero = get_tree().current_scene.get_node("Hero")

func _process(delta):
	# is_instance_valid upewnia się, że obiekt gracza nie został jeszcze usunięty
	if is_instance_valid(hero):
		value = hero.get_hp()
	else:
		# Jeśli gracz zniknął (bo zginął przez queue_free), wymuszamy pusty pasek
		value = 0
