#ifndef JD297_CC_PARSE_TREE_TYPE_C_H
#define JD297_CC_PARSE_TREE_TYPE_C_H

typedef enum ParseTreeType_C {
    PTT_C_TRANSLATION_UNIT,
    PTT_C_EXTERNAL_DECLARATION,
    PTT_C_FUNCTION_DEFINITION,
    PTT_C_DECLARATION,
    PTT_C_DECLARATION_SPECIFIER,
    PTT_C_DECLARATOR,
    PTT_C_COMPOUND_STATEMENT,
    PTT_C_STORAGE_CLASS_SPECIFIER,
    PTT_C_TYPE_SPECIFIER,
    PTT_C_TYPE_QUALIFIER,
    PTT_C_STRUCT_OR_UNION_SPECIFIER,
    PTT_C_ENUM_SPECIFIER,
    PTT_C_TYPEDEF_NAME,
    PTT_C_STRUCT_OR_UNION,
    PTT_C_IDENTIFIER,
    PTT_C_STRUCT_DECLARATION,
    PTT_C_SPECIFIER_QUALIFIER,
    PTT_C_STRUCT_DECLARATOR_LIST,
    PTT_C_STRUCT_DECLARATOR,
    PTT_C_CONSTANT_EXPRESSION,
    PTT_C_POINTER,
    PTT_C_DIRECT_DECLARATOR,
    PTT_C_PARAMETER_TYPE_LIST,
    PTT_C_CONDITIONAL_EXPRESSION,
    PTT_C_LOGICAL_OR_EXPRESSION,
    PTT_C_EXPRESSION,
    PTT_C_LOGICAL_AND_EXPRESSION,
    PTT_C_INCLUSIVE_OR_EXPRESSION,
    PTT_C_EXCLUSIVE_OR_EXPRESSION,
    PTT_C_AND_EXPRESSION,
    PTT_C_EQUALITY_EXPRESSION,
    PTT_C_RELATIONAL_EXPRESSION,
    PTT_C_SHIFT_EXPRESSION,
    PTT_C_ADDITIVE_EXPRESSION,
    PTT_C_MULTIPLICATIVE_EXPRESSION,
    PTT_C_CAST_EXPRESSION,
    PTT_C_UNARY_EXPRESSION,
    PTT_C_TYPE_NAME,
    PTT_C_POSTFIX_EXPRESSION,
    PTT_C_UNARY_OPERATOR,
    PTT_C_PRIMARY_EXPRESSION,
    PTT_C_ASSIGNMENT_EXPRESSION,
    PTT_C_CONSTANT,
    PTT_C_STRING,
    PTT_C_INTEGER_CONSTANT,
    PTT_C_CHARACTER_CONSTANT,
    PTT_C_FLOATING_CONSTANT,
    PTT_C_ENUMERATION_CONSTANT,
    PTT_C_ASSIGNMENT_OPERATOR,
    PTT_C_ABSTRACT_DECLARATOR,
    PTT_C_PARAMETER_LIST,
    PTT_C_PARAMETER_DECLARATION,
    PTT_C_DIRECT_ABSTRACT_DECLARATOR,
    PTT_C_ENUMERATOR_LIST,
    PTT_C_ENUMERATOR,
    PTT_C_INIT_DECLARATOR,
    PTT_C_INITIALIZER,
    PTT_C_INITIALIZER_LIST,
    PTT_C_STATEMENT,
    PTT_C_LABELED_STATEMENT,
    PTT_C_EXPRESSION_STATEMENT,
    PTT_C_SELECTION_STATEMENT,
    PTT_C_ITERATION_STATEMENT,
    PTT_C_JUMP_STATEMENT,
} ParseTreeType_C;

#endif
