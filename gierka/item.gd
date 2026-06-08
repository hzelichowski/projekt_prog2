extends Area2D

func _on_body_entered(body):
	# Sprawdzamy, czy obiekt, który nas dotknął, ma funkcję z C++
	if body.has_method("collect_item"):
		body.collect_item("Magiczny Napoj", 1) # Dodajemy 1 sztukę do plecaka
		Global.destroyed_objects.append(name) # Zapisz moją nazwę!
		queue_free()
