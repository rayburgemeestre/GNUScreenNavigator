#include "menu_renderer.h"
#include <cstring>

#define STATE_WAITING 0
#define STATE_ACTION_MODE 1
#define STATE_NEXT_MENU_REQUEST 2
#define STATE_PREVIOUS_MENU_REQUEST 3
#define STATE_SELECT_ITEM 4

void print_in_middle(WINDOW *win, int starty, int startx, int width, const char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

MenuRenderer::MenuRenderer()
{
	engine = NULL;
	setFastItemSelect(false);

	string selectedItem = "";
}

void MenuRenderer::setMenuName(string newMenuName)
{
	menuName = newMenuName;
}

void MenuRenderer::resetMenuItems()
{
	items.clear();
}

void MenuRenderer::addMenuItem(string key, string menuName)
{
	items[key] = menuName;
}

void MenuRenderer::setEngine(IEngine *pEngine)
{
	engine = pEngine;
}

int MenuRenderer::Render()
{
	assert(engine != NULL);
	
	_initNcurses();
	_initColors();
	_initWindowSizes();
	_initMenuItems();
	_initMenuWindow();


	_initWindow();

	_drawInformationText();
	_drawWindow();

	_runMenu();

	_unloadMenu();
	_unloadWindow();

	// respond to new state
	switch (state) {
		case STATE_PREVIOUS_MENU_REQUEST:
			engine->onPreviousMenuRequest();
			break;
		case STATE_NEXT_MENU_REQUEST:
			engine->onNextMenuRequest();
			break;
		case STATE_SELECT_ITEM:
			engine->onItemSelect(selectedItem);
			break;
	}

	return 0;
}

void MenuRenderer::setFastItemSelect(bool value)
{
	fastItemSelect = value;
}

bool MenuRenderer::isFastItemSelect()
{
	return fastItemSelect;
}

void MenuRenderer::_initNcurses()
{
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}
void MenuRenderer::_initColors()
{
	init_pair(COLORS_MENU_ITEM_ACTIVE, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLORS_MENU_ITEM_INACTIVE , COLOR_WHITE, COLOR_BLACK);
	init_pair(COLORS_MENU_HEADER_TEXT, COLOR_WHITE, COLOR_RED);
	init_pair(COLORS_MENU_HEADER_BACKGROUND, COLOR_RED, COLOR_RED);
}
void MenuRenderer::_initWindowSizes()
{
	// window margin
	windowMarginLeft = (int)(0.1667 * COLS);
	windowMarginRight = (int)(0.1667 * COLS);
	windowMarginTop = (int)(0.1875 * LINES);
	windowMarginBottom = (int)(0.1875 * LINES);

	// window size
	windowWidth = COLS - 2 - windowMarginLeft - windowMarginRight;
	windowHeight = LINES - 6 - windowMarginTop - windowMarginBottom;
	subWindowWidth = windowWidth - 2;
	subWindowHeight = windowHeight - 4;
}
void MenuRenderer::_initMenuItems()
{
    /* add trailing whitespace for more fancy menu items (100% width lines) */
    for (
        itemIterator = items.begin();
        itemIterator != items.end();
        itemIterator++
    ) {
        string key = itemIterator->first;
        int availableLength = (COLS - windowMarginLeft - windowMarginRight - 7/*window border&margin,menu prefix*/);
        int numOfTrailingSpaces = availableLength - items[key].length();
        if (numOfTrailingSpaces > 0)
            items[key].append(numOfTrailingSpaces, ' ');
        
        if (items[key].length() > static_cast<unsigned int>(availableLength)) {
            int centerCharacter = (availableLength) / 2;
            items[key] = items[key].substr(0, centerCharacter - 5) + "..." + 
                         items[key].substr(items[key].length() - centerCharacter);
        }
    }
}
void MenuRenderer::_initMenuWindow()
{
	/* Initialize items */
	int n_choices = items.size();
	menu1items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

	int i = 0;
    for (
        itemIterator = items.begin();
        itemIterator != items.end();
        itemIterator++
    ) {
		string key = itemIterator->first;
		string menuLabel = itemIterator->second;

		menu1items[i++] = new_item(key.c_str(), menuLabel.c_str());
        //root[itemIterator->first] = (itemIterator->second).toJson();
    }

	menu1items[n_choices] = (ITEM *)NULL;

	/* Create menu */
	menu1 = new_menu((ITEM **)menu1items);

	/* Set fore ground and back ground of the menu */
	set_menu_fore(menu1, COLOR_PAIR(COLORS_MENU_ITEM_ACTIVE) | A_REVERSE);
	set_menu_back(menu1, COLOR_PAIR(COLORS_MENU_ITEM_ACTIVE));
	set_menu_grey(menu1, COLOR_PAIR(COLORS_MENU_ITEM_ACTIVE));
}
void MenuRenderer::_initWindow()
{
	// Post menu on a Window
	// .. window height, width, top, left
	window1 = newwin(windowHeight, windowWidth, 1 + windowMarginTop, 1 + windowMarginLeft);
	// .. inside of window height, width
	set_menu_sub(menu1, derwin(window1, subWindowHeight, subWindowWidth, 3, 1));
	// .. display <num> and scroll the rest, <num of columns>
    set_menu_format(menu1, subWindowHeight - 2, 1);


	set_menu_win(menu1, window1);
	//..border
	box(window1, 0, 0);
}
void MenuRenderer::_drawInformationText()
{
	/* Post the menu */
	mvprintw(0, 0, "Launcher v1.0-beta 2007 - http://ray.burgemeestre.net");
	mvprintw(LINES - 3, 0, "NAVIGATE using arrow- or vim- keys {h,j,k,l}");
	mvprintw(LINES - 2, 0, "SELECT ITEM using <Enter> key or directly using shortcut keys {a,b,c,...}");
	mvprintw(LINES - 1, 0, "EXIT program using <Control+C>");
}

void MenuRenderer::_drawWindow()
{
	// Render border with 
	// Append trailing whitespaces (i like the effect of a 100% spanned width..)
	string borderString = "";
	for (int i=0; i < (COLS - 4/*borders+margin*/ - windowMarginLeft - windowMarginRight); i++ ) {
		borderString = borderString + "*";
	}
	print_in_middle(window1, 1, 1, borderString.length(), borderString.c_str(), COLOR_PAIR(COLORS_MENU_HEADER_BACKGROUND));
	// Render text, aligned left
	print_in_middle(window1, 1, 1, menuName.length(), menuName.c_str(), COLOR_PAIR(COLORS_MENU_HEADER_TEXT));
	// Render text, aligned center
	//print_in_middle(window1, 1, 0, COLS, menuName.c_str(), COLOR_PAIR(4));

	post_menu(menu1);
	refresh();
    wrefresh(window1);
}

void MenuRenderer::_runMenu()
{
	state = STATE_WAITING;
	int c = 0;
	while ( (state == STATE_WAITING || state == STATE_ACTION_MODE) 
			&& (c = getch()) != KEY_F(1)
	) {
		switch (c) {
			case '-':
				break;
			case '+':
				break;
			case KEY_RIGHT:
			case 'l':
				state = STATE_NEXT_MENU_REQUEST;
				break;
			case KEY_LEFT:
			case 'h':
				state = STATE_PREVIOUS_MENU_REQUEST;
				break;
			case KEY_DOWN:
			case 'j':
				menu_driver(menu1, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
			case 'k':
				menu_driver(menu1, REQ_UP_ITEM);
				break;
			case 10: // Enter
			case ' ': // Spacebar
				move(20, 0);
				clrtoeol();

				selectedItem = item_name(current_item(menu1));
				mvprintw(1, 0, "MenuItem currently selected is: (key %s) %s ", 
					item_name(current_item(menu1)),
					item_description(current_item(menu1))
				);
				pos_menu_cursor(menu1);

				state = STATE_SELECT_ITEM;
				break;
			default:
				char key[64] = "";
				sprintf(key, "%c", c);
				
				string selected = "";
				engine->onMenuKeyPress(key, selected);
				if (selected != "") {
					// selected contains the selected index
					menu_driver(menu1, REQ_FIRST_ITEM);
					while (item_name(current_item(menu1)) != selected) {
						menu_driver(menu1, REQ_DOWN_ITEM);
					}
				}
				
				if (isFastItemSelect()) {
					selectedItem = item_name(current_item(menu1));
					state = STATE_SELECT_ITEM;
				}
				break;
		}
		wrefresh(window1);
	}
	return;
}

void MenuRenderer::_unloadMenu()
{
	unpost_menu(menu1);
	int n_choices = items.size();
	for(int i = 0; i < n_choices; ++i)
		free_item(menu1items[i]);
	free_menu(menu1);
}

void MenuRenderer::_unloadWindow()
{
	endwin();
}
