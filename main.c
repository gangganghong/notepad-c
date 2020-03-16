#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <strings.h>

#define DEBUG 0
#define FILE_IS_EMPTY "文件为空，请先增加内容"

const int MAX_LEN = 200;
char *test = "abc";

struct line {
    char text[MAX_LEN];
    int num;    // 行号
    struct line *prior;  // 上一行
    struct line *next;   // 下一行
};

struct line *start, *last;
int lineNum;
int targetLineNum;
char *filename;

// 载入文件
// OK
void loadFile(char *filename);

// 一级菜单
int firstMenu();

// 插入菜单
int insertMenu();

// 操作
void action(int menu);

// 退出
void exitAction();

// 显示一行
void displayLine(int lineNum);

// 删除一行
void deleteLine(int lineNum);

// 查找一行
// OK
struct line *findLineBy(int lineNum);

// 保存数据
void save(char *filename, char *mode);

// 显示所有内容
void displayAll();

// 插入功能
void insertFunction();

// 在文件开头插入数据
void insert();

// 追加数据
void add();

// 在指定的行插入数据
void insertInLineNum();

// 在指定行插入一行数据
void insertOneLine();

// 在指定行插入空白行
void insertEmptyLine();

// 获取文件中的所有数据
char *getStrFromFile();

int main() {
    filename = "/Users/cg/data/code/wheel/c/notepad/t";
    do {
        if (DEBUG) {
            time_t lt = time(NULL);
            printf("0==%s==time====%ld\n", __FUNCTION__, lt);
        }
        loadFile(filename);

        int c = firstMenu();
        action(c);
    } while (1);

    return 0;
}

void action(int menu) {
    char *targetLineNumTmp = (char *) malloc(sizeof(char) * 3);
    switch (menu) {
        case 1:
            // 显示
            printf("请输入行号：\n");
            scanf("%s", targetLineNumTmp);
            targetLineNum = atoi(targetLineNumTmp);
            displayLine(targetLineNum);
            break;
        case 2:
            // 插入
            insertFunction();
            break;
        case 3:
            // 删除
            printf("请输入行号：\n");
            scanf("%s", targetLineNumTmp);
            targetLineNum = atoi(targetLineNumTmp);
            deleteLine(targetLineNum);
            if (DEBUG) {
                time_t lt = time(NULL);
                printf("3==%s==time====%ld\n", __FUNCTION__, lt);
            }
            break;
        case 4:
            //保存
            break;
        case 5:
            // 退出
            exitAction();
            break;
        case 6:
            displayAll();
            break;
        case 7:
            loadFile(filename);
            break;
        case 21:
            insert();
            break;
        case 22:
            add();
            break;
        case 24:
            insertInLineNum();
            break;
        case 25:
            insertOneLine();
            break;
        case 26:
            insertEmptyLine();
            break;
        default:
            printf("菜单输入错误\n");
    }
}

void exitAction() {
    printf("退出\n");
    exit(0);
}

// todo 优化。低效。
void displayLine(int lineNum) {
    if (start == NULL) {
        printf("%s\n", FILE_IS_EMPTY);
        return;
    }
    if (DEBUG) {
        printf("lineNum = %d\n", lineNum);
    }
    struct line *targetLine = findLineBy(lineNum);
    if (targetLine) {
        printf("%d\t%s", lineNum, targetLine->text);
    } else {
        printf("错误行数%d\n", lineNum);
    }
}

struct line *findLineBy(int linenum) {
    struct line *info6;
    info6 = start;
    while (info6) {
        if (linenum != info6->num) {
            printf("666%s#####text:%s\n", __FUNCTION__, info6->text);
            printf("666%s#####num:%d\n", __FUNCTION__, info6->num);
            info6 = info6->next;
        } else {
            printf("777%s#####text:%s\n", __FUNCTION__, info6->text);
            printf("777%s#####num:%d\n", __FUNCTION__, info6->num);
            break;
        }
    }
    printf("2222%s#####text:%s\n", __FUNCTION__, info6->text);
    printf("2222%s#####num:%d\n", __FUNCTION__, info6->num);
    return (info6);
}

//struct line *findLineBy(int lineNum) {
//    struct line *targetLine = NULL;
////    struct line *info = start;
//    struct line *info = (struct line *) malloc(sizeof(struct line));
//    info = start;
//    printf("000%s#####text:%s\n", __FUNCTION__, start->text);
//    while (info) {
//        if (info->num == lineNum) {
//            targetLine = info;
//            break;
//        }
//        info->next = (struct line *) malloc(sizeof(struct line));
//        if (!info->next) {
//            printf("内存已经用完\n");
//            exit(0);
//        }
//        info = info->next;
//    }
//    printf("1111%s#####text:%s\n", __FUNCTION__, start->text);
//    printf("33333%s#####text:%s\n", __FUNCTION__, targetLine->text);
//    free(info);   // 又导致野指针
////    main(99051,0x10de32dc0) malloc: *** error for object 0x7fe023d00000: pointer being freed was not allocated
////    main(99051,0x10de32dc0) malloc: *** set a breakpoint in malloc_error_break to debug
//    printf("2222%s#####text:%s\n", __FUNCTION__, targetLine->text);
//    return targetLine;
//}

// todo 没有处理行数
void deleteLine(int lineNum) {
    if (start == NULL) {
        printf("%s\n", FILE_IS_EMPTY);
        return;
    }
    char *mode = "w";
    struct line *targetLine = findLineBy(lineNum);
    // 首行
    if (lineNum == 1) {
        start = (struct line *) malloc(sizeof(struct line));
        start = targetLine->next;
        if (start) {
            start->prior = NULL;
        }
        save(filename, mode);
        return;
    }
    // 尾行
    if (lineNum == last->num) {
        last = targetLine->prior;
        last->next = NULL;
        save(filename, mode);
        return;
    }
    // 其他
    if (targetLine->prior->num == start->num) {
        start->next = targetLine->next;
    } else if (targetLine->next->num == last->num) {
        last->prior = targetLine->prior;
    } else {
        targetLine->prior->next = targetLine->next;
        targetLine->next->prior = targetLine->prior;
    }
    save(filename, mode);
    return;
}

void save(char *filename, char *mode) {
    if (!mode) {
        mode = "w";
    }
    FILE *fp = fopen(filename, mode);
    char *str;
    struct line *info = start;
    while (info) {
        str = info->text;
        while (*str) {
            fputc(*str, fp);
            str++;
        }
//        fputc('\n', fp);
        info = info->next;
    }
    fclose(fp);
//    free(info);
//    free(str);
}

int firstMenu() {
    int no;
    char *c = (char *) malloc(sizeof(char) * 100);
    while (1) {
        printf("\n\t\t1.显示\n");
        printf("\t\t2.插入\n");
        printf("\t\t3.删除\n");
        printf("\t\t4.保存\n");
        printf("\t\t5.退出\n");
        printf("\t\t6.显示全部\n");
        printf("\t\t7.打开\n");
//        printf("\t\t请选择一项功能：\n");

        printf("\t\t21.在开头插入数据\n");
        printf("\t\t22.追加数据\n");
        printf("\t\t23.返回上级菜单\n");
        printf("\t\t24.在选定行插入数据\n");
        printf("\t\t25.在指定行插入一行数据\n");
        printf("\t\t26.在指定行插入空白行\n");
        printf("\t\t请按数字选择：\n");

        scanf("%s", c);
        no = atoi(c);
        printf("no = %d=============\n", no);
        if ((1 <= no && no <= 99)) {
            if (no == 2) {
                // 进入插入菜单
                int res = insertMenu(no);
                // 3 是返回上级菜单
                if (res == 3) {
                    continue;
                }
            }
            break;
        }
    }
    return no;
}

int insertMenu() {
    while (1) {
//        int c = (int)malloc(sizeof(c));
        char *c2;
        int c;
        printf("\n\t1.插入一行文字\n");
        printf("\t2.插入一段文字\n");
        printf("\t3.返回上级菜单\n");
        printf("\t请按数字选择：\n");
//        scanf("%s", c2);
        scanf("%d", &c);
//        printf("%ld\n", time(NULL));
//        printf("c2 = %s\n", c2);
//        int c = atoi(c2);
        if (1 <= c && c <= 3) {
            printf("break\n");
            return c;
        }
    }
}

void loadFile(char *filename) {
    lineNum = 0;
    struct line *tmp;
    struct line *start2;
    char c;
    FILE *fp = fopen(filename, "r");
    tmp = NULL;
    struct line *info = (struct line *) malloc(sizeof(struct line));
    while ((c = fgetc(fp)) != EOF) {
        if (tmp) {
            printf("%d######text:%s\n", tmp->num, tmp->text);
        } else {
            printf("tmp is NULL\n");
        }

        int i = 0;
        if (c == '\n') {
            info->text[i] = '\n';
        } else {
            info->text[i] = c;
            i++;
            // todo 需要检测c==EOF吗？
            while ((c = fgetc(fp)) != '\n' && c != EOF) {
                info->text[i] = c;
                i++;
            }
            if (c == EOF) {
                printf("EOF:%d\n", c);
            }
            // 空行如何识别？空行是10#10
            info->text[i] = '\n';
        }
        lineNum++;
        info->num = lineNum;
        info->next = (struct line *) malloc(sizeof(struct line));
        if (!info->next) {
            printf("\n内存已经用完了\n");
            exit(0);
        }
        info->prior = tmp;
        tmp = info;
        if (start == NULL) {
            start = info;
        }

        info = info->next;
    }
    if (tmp == NULL) {
        fclose(fp);
        start = NULL;
        last = NULL;
        return;
    }

    tmp->next = NULL;
    last = tmp;

    struct line *info6;
//    start = start2;
    start->prior = NULL;
    info6 = start;
    printf("===========info6 start============\n");
    while (info6) {
        printf("num:%d######text:%s\n", info6->num, info6->text);
//        printf("num:%d######text:%s\n", info6->next->num, info6->next->text);
//        printf("num:%d######text:%s\n", info6->next->next->num, info6->next->next->text);
//    printf("num:%d######text:%s\n", last->num, last->text);
        info6 = info6->next;
    }
    printf("===========info6 end============\n");
//    printf("===========info6 start============\n");
//    printf("num:%d######text:%s\n", info6->num, info6->text);
//    printf("num:%d######text:%s\n", info6->next->num, info6->next->text);
//    printf("num:%d######text:%s\n", info6->next->next->num, info6->next->next->text);
////    printf("num:%d######text:%s\n", last->num, last->text);
//    printf("===========info6 end============\n");



//    while (info6) {
//        printf("222last%s#####text:%s\n", __FUNCTION__, last->text);
//        printf("222last-num%s#####num:%d\n", __FUNCTION__, last->num);
//        printf("2222222666%s#####text:%s\n", __FUNCTION__, info6->text);
//        printf("222222222222s666%s#####num:%d\n", __FUNCTION__, info6->num);
//        info6 = info6->next;
//    }





    free(info);
    fclose(fp);
    printf("%s####start:%d\n", __FUNCTION__, start->num);
}

void displayAll() {
    if (start == NULL) {
        printf("%s\n", FILE_IS_EMPTY);
        return;
    }
    struct line *info;
    info = start;
    while (info) {
        printf("%s", info->text);
        info = info->next;
    }
}

void insertFunction() {
    int type = insertMenu();
    switch (type) {
        case 21:
            // 插入一行文字
            insert();
            break;
        case 22:
            // 插入一段文字
            break;
        case 23:
            // 返回上级菜单
            break;
    }
}

void insert() {
    char *str = (char *) malloc(sizeof(char) * MAX_LEN);
    printf("输入数据：\n");
    scanf("%s", str);
    char *str2 = getStrFromFile();
    FILE *fp = fopen(filename, "w");
    fputs(str, fp);
    fgets(str, MAX_LEN, stdin);
//    gets(str);
    fputs(str, fp);
//    fputc('\n', fp);

    printf("str:%s\n", str);
    printf("str2:%s\n", str2);
    fputs(str2, fp);
    fclose(fp);
//    save(filename, "a");
    free(str);
}

void add() {
    char *str = (char *) malloc(sizeof(char) * MAX_LEN);
    printf("请输入数据：\n");
    scanf("%s", str);
    FILE *fp = fopen(filename, "a");
    fputs(str, fp);
    fgets(str, MAX_LEN, stdin);
    fputs(str, fp);
    fclose(fp);
    free(str);
}

void insertInLineNum() {
    if (start == NULL) {
        printf("%s\n", FILE_IS_EMPTY);
        return;
    }
    printf("请输入行数：");
    int lineNum;
    scanf("%d", &lineNum);
    printf("请输入列数：\n");
    int rowNum;
    scanf("%d", &rowNum);
    printf("请输入数据：\n");
    struct line *targetLine = findLineBy(lineNum);
    char *text = targetLine->text;
    int textRowNum = targetLine->num;
    int textLength = strlen(text);

    // 要插入的位置比目标行的长度长
    if (rowNum > textLength) {
        char *str = (char *) malloc(sizeof(char) * MAX_LEN);
        scanf("%s", str);
        char *emptyStr = " ";
        // 去掉换行符
        strcpy(&text[textLength - 1], emptyStr);
        for (int i = 0; i < rowNum - textLength - 1; i++) {
            strcat(text, emptyStr);
        }
        strcat(text, str);
        char *str2 = (char *) malloc(sizeof(char) * MAX_LEN);
        fgets(str2, MAX_LEN, stdin);
        strcat(text, str2);
        // free 操作是否必要？
//        free(emptyStr);
//        free(str);
//        free(str2);
    } else {// 要插入的位置在目标行的中间
        char *str = (char *) malloc(sizeof(char) * MAX_LEN);
        scanf("%s", str);
        char *cache = (char *) malloc(sizeof(char) * MAX_LEN);
        strcpy(cache, &text[rowNum - 1]);
        strcpy(&text[rowNum - 1], str);
        // fgets与上面的scanf，若都使用str，此处会出现Segmentation fault: 11。
        // 在其他地方，同样场景，又不会出现此错误。原因未知。
        char *str2 = (char *) malloc(sizeof(char) * MAX_LEN);
        fgets(str2, MAX_LEN, stdin);
        char trimedLFStr[strlen(str2) - 1];
        int i = 0;
        while (*str2 != '\n') {
            trimedLFStr[i++] = *str2++;
        }
        strcat(text, trimedLFStr);
        strcat(text, cache);
        free(str);
        str = NULL;
        free(cache);
        cache = NULL;
    }
    // 文件存盘
    save(filename, "w");
    // 目标行已经被修改，需要把它放进原来的链表中吗？回答是：不需要。
}

void insertOneLine() {
    if (start == NULL) {
        printf("%s\n", FILE_IS_EMPTY);
        return;
    }
    printf("输入行号:\n");
    int lineNum;
    scanf("%d", &lineNum);
//    return;
    struct line *targetLine = findLineBy(lineNum);
    if (targetLine == NULL) {
        printf("不存在%d行\n", lineNum);
        return;
    }
    printf("输入数据:\n");
//    char *str = NULL;
    char *str = (char *) malloc(sizeof(char) * MAX_LEN);
    scanf("%s", str);
//    char *str2 = NULL;
    char *str2 = (char *) malloc(sizeof(char) * MAX_LEN);
    printf("0str2 = %s\n", str2);
    fgets(str2, MAX_LEN, stdin);
    char *newStr = (char *) malloc(sizeof(char) * MAX_LEN);
    strcat(newStr, str);
    strcat(newStr, str2);
    printf("str = %s\nstr2 = %s\n", str, str2);
    struct line *newLine = (struct line *) malloc(sizeof(struct line));
    strcpy(newLine->text, newStr);
    printf("newStr = %s\n", newStr);
    // 此处的链表操作，容易出现死循环
    newLine->prior = targetLine->prior;
    printf("targetLine->prior:%p\n", targetLine->prior);
    newLine->next = targetLine;
    if (targetLine->prior) {
        targetLine->prior->next = newLine;
    }
    targetLine->prior = newLine;
    if (targetLine->num == 1) {
        start = newLine;
    }
    free(newLine);
    newLine = NULL;
    printf("2222222\n");

    struct line *info2;
    info2 = start;
    int i = 0;
    while (info2) {
        printf("info->num:%d###info->text:%s\n", info2->num, info2->text);
        info2 = info2->next;
        i++;
        if (i == 4) {
            break;
        }
    }
    save(filename, "w");
}

void insertEmptyLine() {
    if (start == NULL) {
        printf("%s\n", FILE_IS_EMPTY);
        return;
    }
    int lineNum;
    printf("输入行号：\n");
    scanf("%d", &lineNum);
//    lineNum = 2;
    struct line *targetLine = findLineBy(lineNum);
    if (targetLine == NULL) {
        printf("%s\n", "目标行不存在");
        return;
    }

    struct line *newLine = (struct line *) malloc(sizeof(struct line));
    newLine->text[0] = '\n';
//    newLine->text[1] = '\0';

    // 在首行插入
    if (start->num == targetLine->num) {
        newLine->next = targetLine;
        start = newLine;
    } else {
        newLine->next = targetLine;
        newLine->prior = targetLine->prior;
        targetLine->prior->next = newLine;
        targetLine->prior = newLine;
    }
//
    struct line *info4;
//    struct line *info4 = (struct line *) malloc(sizeof(struct line));
    info4 = start;
    while (info4) {
        printf("info4:%s\n", info4->text);
        info4 = info4->next;
    }

    save(filename, "w");
}

char *getStrFromFile() {
    char *str = (char *) malloc(sizeof(char) * MAX_LEN);
    if (start == NULL) {
        return str;
    }
    struct line *info;
    info = start;
    while (info) {
        strcat(str, info->text);
        info = info->next;
        if (info) {
            str = (char *) realloc(str, sizeof(char) * MAX_LEN);
        }
    }

    return str;
}