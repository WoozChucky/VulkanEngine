//
// Created by nunol on 12/22/2021.
//

#ifndef THIS_FRAMEWORK_CORE_DISPLAY_HPP_
#define THIS_FRAMEWORK_CORE_DISPLAY_HPP_

#include <DataTypes.hpp>

class Display {
 public:
	Display(S32 width, S32 height, const char* title, const OnKeyCallback& keyCallback, const OnMouseCallback& mouseCallback);
	~Display();
	bool IsClosed() const;
	void Update(bool draw, bool pollEvents);
	void Draw();
	void Clear(R32 red, R32 green, R32 blue, R32 alpha);

	static std::shared_ptr<Display> makeShared();
 private:
	GLFWwindow* window;
	bool isClosed;

	OnKeyCallback onKeyCallback;
	OnMouseCallback mouseCallback;

	S32 height;
	S32 width;

	R64 x;
};

#endif //THIS_FRAMEWORK_CORE_DISPLAY_HPP_
