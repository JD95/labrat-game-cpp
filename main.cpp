// C++ Headers
#include <string>
#include <iostream>
#include <tuple>
#include <functional>
#include <vector>
#include <glm.hpp>
#include <SDL.h>

#include "../labrat/window/game.h"
#include "../labrat/utilities/function_view.h"
#include "../labrat/reactive/reactive.h"
#include "../labrat/utilities/SlotMap.h"
#include "../labrat/entity/entity.h"

#include "../labrat/reactive/source.h"

#include "Level1/Level1.h"

using std::vector;
using std::function;

void print_id_list(SlotMap<Entity>& entities) {
	std::cout << "In use ID's\n";
	for (int i = 0; i < entities.first_free_index; i++)
	{
		std::cout << entities.id_list[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Free ID's\n";
	for (int i = entities.first_free_index; i < entities.first_free_index + 10; i++)
	{
		std::cout << entities.id_list[i] << " ";
	}
	std::cout << std::endl;
}

void test_slotmap() {
	SlotMap<Entity> entities;
	auto a = entities.create_object();
	auto b = entities.create_object();
	auto c = entities.create_object();

	print_id_list(entities);

	std::cout << "Destroying entity 1\n";

	entities.destroy_object(b);

	print_id_list(entities);

	std::cout << "Creating two more entities\n";

	auto d = entities.create_object();
	auto e = entities.create_object();

	print_id_list(entities);

	std::cout << "Trying to access destroyed entity\n";

	std::cout << (entities.get_object(b) == nullptr ? "Entity version destroyed successfully!\n" : "Entity version was not incremented!\n");

	system("pause");
}

int test(int x, int y) { return x + y; }

void test_reactive_values() {
	Reactive<int> x(5);
	Reactive<int> y(5);
	Reactive<int> z(0);

	Reactive<std::string> foo("foo");
	Reactive<std::string> bar("bar");

	auto source = 
		from(x, y)
		.use([](int x, int y) { return x + y; })
		.determine(z);

	auto str_source =
		from(foo, bar)
		.use([](std::string f, std::string b) { return f + b; })
		.determine(foo);

	std::vector<std::unique_ptr<Updater>> ds;

	ds.push_back(std::move(source));
	ds.push_back(std::move(str_source));

	while (true) {
		x = 0;
		for(auto& d : ds) d->calculate();
		for (auto& d : ds) d->update();
		std::cout << z.value << std::endl;
		std::cout << foo.value << std::endl;
		system("pause");
	}
}

int main(int argc, char *argv[])
{
	//test_slotmap();
	//Game<Level1> super_alpha(std::string("labrat Engine - Super Alpha"), 1080,720);
	//super_alpha.game();

	test_reactive_values();

	return 0;
}