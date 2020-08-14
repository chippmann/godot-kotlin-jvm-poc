extends Node2D


# Declare member variables here. Examples:
# var a: int = 2
# var b: String = "text"


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	benchmark(funcref(get_child(0), "benchmarkSimpleAdd"), "benchmarkSimpleAdd")
	benchmark(funcref(get_child(0), "benchmarkAvg"), "benchmarkAvg")
	benchmark(funcref(get_child(0), "benchmarkSimpleAdd"), "benchmarkSimpleAdd")
	benchmark(funcref(get_child(0), "benchmarkAvg"), "benchmarkAvg")
	benchmark(funcref(get_child(0), "benchmarkSimpleAdd"), "benchmarkSimpleAdd")
	benchmark(funcref(get_child(0), "benchmarkAvg"), "benchmarkAvg")
	benchmark(funcref(get_child(0), "benchmarkSimpleAdd"), "benchmarkSimpleAdd")
	benchmark(funcref(get_child(0), "benchmarkAvg"), "benchmarkAvg")


func benchmark(function: FuncRef, functionName: String):
	var durations = []
	for iteration in range(1000000):
		var start = OS.get_ticks_usec()
		function.call_func()
		durations.append(OS.get_ticks_usec() - start)
	
	var avg: float = 0
	for duration in durations:
		avg += duration
	avg /= durations.size()
	
	var ops = 1000000 / avg
	print("Benchmark: %s" % functionName)
	print("Results: avgDur(usec) = %s, op/s=%s" % [avg, ops])
