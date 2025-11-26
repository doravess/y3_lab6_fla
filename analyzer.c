// analyzer.c
#include <stdio.h>
#include <stdlib.h>
#include <clang-c/Index.h>

// Глобальні лічильники
int g_loop_depth = 0;
int g_if_in_loop_count = 0;


CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    // отримання типу вузла AST
    enum CXCursorKind kind = clang_getCursorKind(cursor);

    // перевірна входу та виходу з цикла
    if (kind == CXCursor_ForStmt ||
        kind == CXCursor_WhileStmt ||
        kind == CXCursor_DoStmt)
    {
        // цикл, збільшимо глибину
        g_loop_depth++;

        clang_visitChildren(cursor, visitor, NULL);

        // вихід з циклу
        g_loop_depth--;


        return CXChildVisit_Continue;
    }

    // перевірка if
    if (kind == CXCursor_IfStmt) {
        // перевірка, чи всередині циклу наше if
        if (g_loop_depth > 0) {
            g_if_in_loop_count++;

            CXString cursor_spelling = clang_getCursorSpelling(cursor);
            clang_disposeString(cursor_spelling);
        }
    }

    return CXChildVisit_Recurse;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <target_cpp_file>\n", argv[0]);
        return 1;
    }

    // індекс
    CXIndex index = clang_createIndex(0, 0);

    const char *file_name = argv[1];

    const char *clang_args[] = {
            "-xc++",
            "-std=c++20"
    };

    CXTranslationUnit unit = clang_parseTranslationUnit(
            index,
            file_name,
            clang_args,
            sizeof(clang_args) / sizeof(clang_args[0]),
            NULL,
            0,
            CXTranslationUnit_None
    );

    if (unit == NULL) {
        fprintf(stderr, "Unable to parse translation unit.\n");
        clang_disposeIndex(index);
        return 1;
    }

    // 3. Обхід AST, починаючи з кореневого вузла
    CXCursor root_cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(root_cursor, visitor, NULL);

    // 4. Виведення результату
    printf("Result: The number of 'if' statements inside loops in %s is: %d\n",
           file_name,
           g_if_in_loop_count);

    // 5. Очищення ресурсів
    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return 0;
}