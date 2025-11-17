#include <stdio.h>
#include <ctype.h>

#define MAX 100

char stack[MAX];
int top = -1;

// Stack operations
void push(char c) {
    stack[++top] = c;
}
char pop() {
    return stack[top--];
}
char peek() {
    return stack[top];
}
int isEmpty() {
    return top == -1;
}

// Precedence function
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert infix to postfix
void infixToPostfix(char infix[], char postfix[]) {
    int i, k = 0;
    char c;

    for (i = 0; infix[i] != '\0'; i++) {
        c = infix[i];

        if (isdigit(c)) {
            postfix[k++] = c;   // Operand → output
        }
        else if (c == '(') {
            push(c);
        }
        else if (c == ')') {
            while (!isEmpty() && peek() != '(' ) {
                postfix[k++] = pop();
            }
            pop();  // Remove '(' from stack
        }
        else {
            while (!isEmpty() && precedence(peek()) >= precedence(c)) {
                postfix[k++] = pop();
            }
            push(c);
        }
    }

    while (!isEmpty()) {
        postfix[k++] = pop();
    }

    postfix[k] = '\0';  // Null terminate
}

// Evaluate postfix expression
int evaluatePostfix(char postfix[]) {
    int valStack[MAX];
    int valTop = -1;
    int i, op1, op2;

    for (i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (isdigit(c)) {
            valStack[++valTop] = c - '0';   // Push number
        }
        else {
            op2 = valStack[valTop--];
            op1 = valStack[valTop--];

            switch (c) {
                case '+': valStack[++valTop] = op1 + op2; break;
                case '-': valStack[++valTop] = op1 - op2; break;
                case '*': valStack[++valTop] = op1 * op2; break;
                case '/': valStack[++valTop] = op1 / op2; break;
            }
        }
    }

    return valStack[valTop];
}

int main() {
    char infix[MAX] = "(3+4)*2";
    char postfix[MAX];

    printf("Infix Expression: %s\n", infix);

    infixToPostfix(infix, postfix);
    printf("Postfix Expression: %s\n", postfix);

    int result = evaluatePostfix(postfix);
    printf("Evaluation Result: %d\n", result);

    return 0;
}
