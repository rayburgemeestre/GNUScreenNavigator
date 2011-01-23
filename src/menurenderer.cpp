#include "menurenderer.h"
#include "abstractengine.h"
#include <cstring>
#include <cassert>


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
	engine_ = NULL;
	setFastItemSelect(false);

	string selectedItem_ = "";
}

void MenuRenderer::setMenuName(string menuName)
{
	menuName_ = menuName_;
}

void MenuRenderer::resetMenuItems()
{
	items_.clear();
}

void MenuRenderer::addMenuItem(string key, string menuName)
{
	items_[key] = menuName;
}

void MenuRenderer::setEngine(AbstractEngine *engine)
{
	engine_ = engine;
}

int MenuRenderer::Render()
{
	assert(engine_ != NULL);
	
	initNcurses();
	initColors();
	initWindowSizes();
	initMenuItems();
	initMenuWindow();


	initWindow();

	drawInformationText();
	drawWindow();

	runMenu();

	unloadMenu();
	unloadWindow();

	// respond to new state_
	switch (state_) {
		case STATE_PREVIOUS_MENU_REQUEST:
			engine_->onPreviousMenuRequest();
			break;
		case STATE_NEXT_MENU_REQUEST:
			engine_->onNextMenuRequest();
			break;
		case STATE_SELECT_ITEM:
			engine_->onItemSelect(selectedItem_);
			break;
	}

	return 0;
}

void MenuRenderer::setFastItemSelect(bool fastItemSelect)
{
	fastItemSelect_ = fastItemSelect;
}

bool MenuRenderer::isFastItemSelect()
{
	return fastItemSelect_;
}

void MenuRenderer::initNcurses()
{
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}
void MenuRenderer::initColors()
{
	init_pair(COLORS_MENU_ITEM_ACTIVE, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLORS_MENU_ITEM_INACTIVE , COLOR_WHITE, COLOR_BLACK);
	init_pair(COLORS_MENU_HEADER_TEXT, COLOR_WHITE, COLOR_RED);
	init_pair(COLORS_MENU_HEADER_BACKGROUND, COLOR_RED, COLOR_RED);
}
void MenuRenderer::initWindowSizes()
{
	// window margin
	windowMarginLeft_ = (int)(0.1667 * COLS);
	windowMarginRight_ = (int)(0.1667 * COLS);
	windowMarginTop_ = (int)(0.1875 * LINES);
	windowMarginBottom_ = (int)(0.1875 * LINES);

	// window size
	windowWidth_ = COLS - 2 - windowMarginLeft_ - windowMarginRight_;
	windowHeight_ = LINES - 6 - windowMarginTop_ - windowMarginBottom_;
	subWindowWidth_ = windowWidth_ - 2;
	subWindowHeight_ = windowHeight_ - 4;
}
void MenuRenderer::initMenuItems()
{
    /* add trailing whitespace for more fancy menu items_ (100% width lines) */
    for (itemIterator_ = items_.begin(); itemIterator_ != items_.end(); ++itemIterator_) {
        string key = itemIterator_->first;
        int availableLength = (COLS - windowMarginLeft_ - windowMarginRight_ - 7/*window border&margin,menu prefix*/);
        int numOfTrailingSpaces = availableLength - items_[key].length();
        if (numOfTrailingSpaces > 0)
            items_[key].append(numOfTrailingSpaces, ' ');
        
        if (items_[key].length() > static_cast<unsigned int>(availableLength)) {
            int centerCharacter = (availableLength) / 2;
            items_[key] = items_[key].substr(0, centerCharacter - 5) + "..." + 
                         items_[key].substr(items_[key].length() - centerCharacter);
        }
    }
}
void MenuRenderer::initMenuWindow()
{
	/* Initialize items_ */
	int n_choices = items_.size();
	menu1items_ = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

	int i = 0;
    for (itemIterator_ = items_.begin(); itemIterator_ != items_.end(); ++itemIterator_) {
		string key = itemIterator_->first;
		string menuLabel = itemIterator_->second;
		menu1items_[i++] = new_item(key.c_str(), menuLabel.c_str());
    }
	menu1items_[n_choices] = (ITEM *)NULL;

	/* Create menu */
	menu1_ = new_menu((ITEM **)menu1items_);

	/* Set fore ground and back ground of the menu */
	set_menu_fore(menu1_, COLOR_PAIR(COLORS_MENU_ITEM_ACTIVE) | A_REVERSE);
	set_menu_back(menu1_, COLOR_PAIR(COLORS_MENU_ITEM_ACTIVE));
	set_menu_grey(menu1_, COLOR_PAIR(COLORS_MENU_ITEM_ACTIVE));
}
void MenuRenderer::initWindow()
{
	// Post menu on a Window
	// .. window height, width, top, left
	window1_ = newwin(windowHeight_, windowWidth_, 1 + windowMarginTop_, 1 + windowMarginLeft_);
	// .. inside of window height, width
	set_menu_sub(menu1_, derwin(window1_, subWindowHeight_, subWindowWidth_, 3, 1));
	// .. display <num> and scroll the rest, <num of columns>
    set_menu_format(menu1_, subWindowHeight_ - 2, 1);


	set_menu_win(menu1_, window1_);
	//..border
	box(window1_, 0, 0);
}
void MenuRenderer::drawInformationText()
{
	/* Post the menu */
	mvprintw(0, 0, "Launcher v1.0-beta 2007 - http://ray.burgemeestre.net");
	mvprintw(LINES - 3, 0, "NAVIGATE using arrow- or vim- keys {h,j,k,l}");
	mvprintw(LINES - 2, 0, "SELECT ITEM using <Enter> key or directly using shortcut keys {a,b,c,...}");
	mvprintw(LINES - 1, 0, "EXIT program using <Control+C>");
}

void MenuRenderer::drawWindow()
{
	// Render border with 
	// Append trailing whitespaces (i like the effect of a 100% spanned width..)
	string borderString = "";
	for (int i=0; i < (COLS - 4/*borders+margin*/ - windowMarginLeft_ - windowMarginRight_); i++ ) {
		borderString = borderString + "*";
	}
	print_in_middle(window1_, 1, 1, borderString.length(), borderString.c_str(), COLOR_PAIR(COLORS_MENU_HEADER_BACKGROUND));
	// Render text, aligned left
	print_in_middle(window1_, 1, 1, menuName_.length(), menuName_.c_str(), COLOR_PAIR(COLORS_MENU_HEADER_TEXT));
	// Render text, aligned center
	//print_in_middle(window1_, 1, 0, COLS, menuName_.c_str(), COLOR_PAIR(4));

	post_menu(menu1_);
	refresh();
    wrefresh(window1_);
}

void MenuRenderer::runMenu()
{
	state_ = STATE_WAITING;
	int c = 0;
	while ( (state_ == STATE_WAITING || state_ == STATE_ACTION_MODE) 
			&& (c = getch()) != KEY_F(1)
	) {
		switch (c) {
			case '-':
				break;
			case '+':
				break;
			case KEY_RIGHT:
			case 'l':
				state_ = STATE_NEXT_MENU_REQUEST;
				break;
			case KEY_LEFT:
			case 'h':
				state_ = STATE_PREVIOUS_MENU_REQUEST;
				break;
			case KEY_DOWN:
			case 'j':
				menu_driver(menu1_, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
			case 'k':
				menu_driver(menu1_, REQ_UP_ITEM);
				break;
			case 10: // Enter
			case ' ': // Spacebar
				move(20, 0);
				clrtoeol();

				selectedItem_ = item_name(current_item(menu1_));
				mvprintw(1, 0, "MenuItem currently selected is: (key %s) %s ", 
					item_name(current_item(menu1_)),
					item_description(current_item(menu1_))
				);
				pos_menu_cursor(menu1_);

				state_ = STATE_SELECT_ITEM;
				break;
			default:
				char key[64] = "";
				sprintf(key, "%c", c);
				
				string selected = "";
				engine_->onMenuKeyPress(key, selected);
				if (selected != "") {
					// selected contains the selected index
					menu_driver(menu1_, REQ_FIRST_ITEM);
					while (item_name(current_item(menu1_)) != selected) {
						menu_driver(menu1_, REQ_DOWN_ITEM);
					}
				}
				
				if (isFastItemSelect()) {
					selectedItem_ = item_name(current_item(menu1_));
					state_ = STATE_SELECT_ITEM;
				}
				break;
		}
		wrefresh(window1_);
	}
	return;
}

void MenuRenderer::unloadMenu()
{
	unpost_menu(menu1_);
	int n_choices = items_.size();
	for(int i = 0; i < n_choices; ++i)
		free_item(menu1items_[i]);
	free_menu(menu1_);
}

void MenuRenderer::unloadWindow()
{
	endwin();
}
