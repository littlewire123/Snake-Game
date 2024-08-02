#include "./online_menu.h"

void chioce_room_mod()
{
    int highlight = 0;
    int c;

    while (1)
    {
        system("clear");

        while (1)
        {
            int menu_size;
            system("clear");
            menu_size = print_roommenu(highlight);
            c = getch();

            if (c == 27)
            {
                getch();
                c = getch();

                if (c == UP_ARROW)
                {
                    if (highlight > 0)
                    {
                        --highlight;
                    }
                }
                else if (c == DOWN_ARROW)

                {
                    if (highlight < menu_size - 1)
                    {
                        ++highlight;
                    }
                }
            }
            else if (c == ENTER_KEY)
            {
                chioce_search_mod = highlight + 1;
                break;
            }
        }

        switch (chioce_search_mod)
        {
        case 1:
            system("clear");
            init_online_search();
            continue;
        case 2:
            system("clear");
            chioce_online_mod();
            continue;
        case 3:
            send_game_data();
            continue;
        case 4:
            get_game_data();
            continue;
        case 5:
            return;
        default:
            continue;
        }
    }
    return ;
}

void chioce_online_mod()
{
    int highlight = 0;
    int c;

    while (1)
    {
        system("clear");

        while (1)
        {
            int menu_size;
            system("clear");
            menu_size = print_online_menu(highlight);
            c = getch();

            if (c == 27)
            {
                getch(); // skip the [
                c = getch();

                if (c == UP_ARROW)
                {
                    if (highlight > 0)
                    {
                        --highlight;
                    }
                }
                else if (c == DOWN_ARROW)
                {
                    if (highlight < menu_size - 1)
                    {
                        ++highlight;
                    }
                }
            }
            else if (c == ENTER_KEY)
            {
                chioce_mod_online = highlight + 1;
                break;
            }
        }

        switch (chioce_mod_online)
        {
        case 1:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
                printf("\033[?25h");
            }
            return;
        case 2:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
                printf("\033[?25h");
            }
            return;
        case 3:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
                printf("\033[?25h");
            }
            return;
        case 4:
            return;
        default:
            continue;
        }
    }
    return;
}

void login_online()
{
    int highlight = 0;
    int c;
    int mod = 0;
    if (user_login_info != NULL)
    {
        free(user_login_info);
    }
    user_login_info = (struct user_info_t *)malloc(sizeof(struct user_info_t));
    user_login_info->user_name[0] = '\0';
    user_login_info->user_pwd[0] = '\0';

    while (1)
    {
        enable_raw_mode();
        system("clear");
        int menu_size;
        menu_size = print_loginmenu(highlight);
        c = getchar();
        if (c == 27)
        {
            getch();
            c = getch();

            if (c == UP_ARROW)
            {
                if (highlight > 0)
                {
                    --highlight;
                }
            }
            else if (c == DOWN_ARROW)
            {
                if (highlight < menu_size - 1)
                {
                    ++highlight;
                }
            }
        }
        else if (c == ENTER_KEY)
        {
            if (highlight == 0)
            {
                int rows, cols;
                get_terminal(&rows, &cols);
                int start_row = (rows - menu_size) / 2;
                int start_col = (cols - strlen("input your id")) / 2 + strlen("input your id");

                printf("\033[%d;%dH", start_row, start_col + 1);
                disable_raw_mode();
                printf("\033[K");
                fgets(user_login_info->user_name, 32, stdin);
                user_login_info->user_name[strcspn(user_login_info->user_name, "\n")] = 0; // Remove newline character
                enable_raw_mode();
            }
            else if (highlight == 1)
            {
                int rows, cols;
                get_terminal(&rows, &cols);
                int start_row = (rows - menu_size) / 2 + 1;
                int start_col = (cols - strlen("input your passwd")) / 2 + strlen("input your passwd");

                printf("\033[%d;%dH", start_row, start_col + 1);
                printf("\033[K");
                input_password(user_login_info->user_pwd, 32);
            }
            else if (highlight == 2 || highlight == 3 || highlight == 4)
            {
                mod = highlight + 1;
                break;
            }
        }
        
    }

    disable_raw_mode();

    switch (mod)
    {
    case 3:
        if (signup_flag == 1)
        {
            int ret;
            ret = get_recv_info();
            if (ret == 1 && status != NULL && status->code != 0)
            {
                chioce_room_mod();
            }
            else
            {
                login_online();
            }
        }
        else if (!init_online_login())
        {
            printf("connet flase !\n");
            break;
        }
        else
        {
            chioce_room_mod();
            //login_online();
        }
        // Handle login game
        break ;
    case 4:
        signup_online();
        // Handle sign up for a game
        break;
    case 5:
        return;
    default:
        break;
    }
}

void signup_online()
{
    int highlight = 0;
    int c;
    int mod = 0;
    if (user_login_info != NULL)
    {
        free(user_login_info);
    }
    user_login_info = (struct user_info_t *)malloc(sizeof(struct user_info_t));
    user_login_info->user_name[0] = '\0';
    user_login_info->user_pwd[0] = '\0';

    enable_raw_mode();

    while (1)
    {
        system("clear");
        int menu_size;
        menu_size = print_signupmenu(highlight);
        c = getchar();
        if (c == 27)
        {
            getch();
            c = getch();

            if (c == UP_ARROW)
            {
                if (highlight > 0)
                {
                    --highlight;
                }
            }
            else if (c == DOWN_ARROW)
            {
                if (highlight < menu_size - 1)
                {
                    ++highlight;
                }
            }
        }
        else if (c == ENTER_KEY)
        {
            if (highlight == 0)
            {
                int rows, cols;
                get_terminal(&rows, &cols);
                int start_row = (rows - menu_size) / 2;
                int start_col = (cols - strlen("input your id")) / 2 + strlen("input your id");

                printf("\033[%d;%dH", start_row, start_col + 1);
                disable_raw_mode();
                printf("\033[K");
                fgets(user_login_info->user_name, 32, stdin);
                user_login_info->user_name[strcspn(user_login_info->user_name, "\n")] = 0; // Remove newline character
                enable_raw_mode();
            }
            else if (highlight == 1)
            {
                int rows, cols;
                get_terminal(&rows, &cols);
                int start_row = (rows - menu_size) / 2 + 1;
                int start_col = (cols - strlen("input your passwd")) / 2 + strlen("input your passwd");

                printf("\033[%d;%dH", start_row, start_col + 1);
                printf("\033[K");
                input_password(user_login_info->user_pwd, 32);
            }
            else if (highlight == 2)
            {
                int rows, cols;
                get_terminal(&rows, &cols);
                int start_row = (rows - menu_size) / 2 + 2;
                int start_col = (cols - strlen("input your passwd again")) / 2 + strlen("input your passwd again");

                printf("\033[%d;%dH", start_row, start_col + 1);
                printf("\033[K");
                input_password(passwd_again, 32);
            }
            else if (highlight == 3)
            {

                if (strcmp(passwd_again, user_login_info->user_pwd) != 0)
                {
                    int rows, cols;
                    get_terminal(&rows, &cols);
                    int start_row = (rows - menu_size) / 2 + 3;
                    int start_col = (cols - strlen("The password is inconsistent twice.")) / 2;
                    printf("\033[%d;%dHThe password is inconsistent twice.", start_row, start_col);
                    fflush(stdout);
                    sleep(1);
                    continue;
                }
                else
                {
                    mod = highlight + 1;
                    break;
                }
            }
            else if (highlight == 4)
            {
                mod = highlight + 1;
                break;
            }
        }
    }

    disable_raw_mode();

    switch (mod)
    {
    case 4:
        if (!init_online_SignUp())
        {
            printf("connet flase !\n");
            break;
        }
        else
        {
            signup_flag = 1; // 注册后登录游戏 不需要在连接服务器
            login_online();
        }
        // Handle sign up for a game
        break;
    case 5:
        return;
    default:
        break;
    }
}

int print_online_menu(int highlight)
{
    const char *menu[] = {
        "Classic_online_mode",
        "Challenge_online_mode",
        "Power_up_online_mode",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);

    return menu_size;
}

int print_roommenu(int highlight)
{
    const char *menu[] = {
        "Search for room",
        "Create a room",
        "Saved games",
        "Get saved games",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);

    return menu_size;
}

int print_loginmenu(int highlight)
{
    const char *menu[] = {
        "Input your name",
        "Input your passwd",
        "Login game",
        "Sign up for a game",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu_login(menu, menu_size, highlight, user_login_info->user_name, user_login_info->user_pwd, "");

    return menu_size;
}

int print_signupmenu(int highlight)
{
    const char *menu[] = {
        "Input your name",
        "Input your passwd",
        "Input your passwd again",
        "Sign up",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu_login(menu, menu_size, highlight, user_login_info->user_name, user_login_info->user_pwd, passwd_again);

    return menu_size;
}

void print_centered_menu_login(const char **menu, int menu_size, int highlight, const char *id, const char *passwd, const char *passwdagain)
{
    int rows, cols;
    get_terminal(&rows, &cols);

    int start_row = (rows - menu_size) / 2;
    int i;
    for (i = 0; i < menu_size; ++i)
    {
        int start_col = (cols - strlen(menu[i])) / 2;

        if (highlight == i)
        {
            HIGHLIGHT_TEXT();
            printf("\033[%d;%dH%s", start_row + i, start_col, menu[i]);
            RESET_TEXT();
        }
        else
        {
            YELLOW_TEXT();
            printf("\033[%d;%dH%s", start_row + i, start_col, menu[i]);
            RESET_TEXT();
        }

        if (i == 0 && id[0] != '\0')
        {
            printf("   %s", id);
        }
        else if (i == 1 && passwd[0] != '\0')
        {
            int j;
            printf("  ");
            for (j = 0; j < strlen(passwd); ++j)
            {
                printf("*");
            }
        }
        else if (i == 2 && passwdagain[0] != '\0')
        {
            int j;
            printf("  ");
            for (j = 0; j < strlen(passwdagain); ++j)
            {
                printf("*");
            }
        }
    }
    fflush(stdout);
}

void input_password(char *password, int max_len)
{
    struct termios oldt, newt;
    int i = 0;
    char c;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO); // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (i < max_len - 1 && (c = getchar()) != '\n')
    {
        if (c == 127)
        { // Handle backspace
            if (i > 0)
            {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        }
        else
        {
            password[i++] = c;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate string

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}

void enable_raw_mode()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disable_raw_mode()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
