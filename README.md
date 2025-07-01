# Rust Compiler

## Gramática
```rust
Program ::= StructList FunDecList

StructList ::= (struct id { AttrDecList }) *

AttrDecList ::= AttrDec (, AttrDec) [,]

AttrDec ::= id : Type

FunDecList ::= (fn FunDec)+ 

FunDec ::= id ([ParamDecList]) [-> Type] { Body }

ParamDecList ::= ParamDec (, ParamDec )*

ParamDec ::= [ mut ] id : [&] [ mut ] Type

Type ::= id

Body ::= StmtList

StmtList ::= ( Stmt )*

Stmt ::= id (.id)* (= | += | -= | *= | /= | %=) AExp ; |
    id ( [ArgList] ) ; |
    println!("{}", AExp ) ; |
    print!("{}", AExp ); |
    if AExp { Body } [ else { Body } ] |
    while AExp { Body } |
    for [ mut ] id in AExp..AExp { Body } |
    return [ AExp ] ; |
	let [ mut ] id : Type [ = AExp] ; |
    break ; |
    continue ; 

AExp ::= BExp ((&& | ||) BExp)*

BExp ::= CExp | !CExp

CExp ::= Exp [(<|<=|>|>=|==|!=) Exp]

Exp ::= Term ((+ | -) Term)*

Term ::= PostfixExp ((*|/|%) PostfixExp)*

PostfixExp ::= Factor (. id)*

Factor ::= [-] ([&] id | Num | Bool | ( AExp ) | if AExp {AExp} else {AExp} | id ( [ArgList] ) | StructExp )

StructExp ::= id { StructExpAttr (, StructExpAttr)* [,] }

StructExpAttr ::= id : AExp

ArgList ::= AExp (, AExp)*

Bool ::= true | false
```

## Structs

### Scanner

Se añadieron los siguientes tokens para manejar structs:

| Nombre | Token | Significado |
|--------|-------|-------------|
| STRUCT | struct| Keyword para declarar un struct |
| BORROW | & | Usado para definir que un struct es pasado por referencia a una función |
| DOT | . | Usado para acceder a los atributos de un struct |

### Parser

Se agregó lo necesario a la gramática para manejar structs:

Un programa se compone de una lista de structs y una lista de funciones:
```rust
Program ::= StructList FunDecList
```
Cada struct es la keyword `struct`, seguida de un id y una lista de declaración de atributos:
```rust
StructList ::= (struct id { AttrDecList }) *
```
Una lista de declaración de atributos es varias declaraciones de atributos separadas por comas:
```rust
AttrDecList ::= AttrDec (, AttrDec) [,]
```
Una declaración de atributo es un id, seguido por dos puntos (`:`) y un tipo de dato:
```rust
AttrDec ::= id : Type
```
Un parámetro de una función ahora puede ser un atributo por referencia especificado por `&`:
```rust
ParamDec ::= [ mut ] id : [&] [ mut ] Type
```
Un AssignStatement ahora puede ser un id seguido de otros ids separados por puntos, indicando que se está asignando a un atributo de un struct:
```rust
Stmt ::= id (.id)* (= | += | -= | *= | /= | %=) AExp ;
```
Un term ahora se compone por dos PostfixExp:
```rust
Term ::= PostfixExp ((*|/|%) PostfixExp)*
```
Un PostfixExp se usa para añadirle algo después de un factor. En este caso, se usa para especificar los atributos de un struct:
```rust
PostfixExp ::= Factor (. id)*
```
Un factor ahora puede ser un StructExp:
```rust
Factor ::= [-] ([&] id | Num | Bool | ( AExp ) | if AExp {AExp} else {AExp} | id ( [ArgList] ) | StructExp )
```
Un StructExp es un identificador seguido de los atributos del struct separados por comas:
```rust
StructExp ::= id { StructExpAttr (, StructExpAttr)* [,] }
```
Cada atributo de un struct se define por un identificar, y una expresión que se evaluará para darle valor al atributo:
```rust
StructExpAttr ::= id : AExp
```

### Check Visitor

### GenCode Visitor

A la hora de evaluar un StructExp, se reserva memoria suficiente para guadar el struct temporalmente, y se guarda en `struct_offset` su posición relativa a %rbp:
```cpp
ImpType GenCodeVisitor::visit(StructExp* e) {
    int current_struct_offset = temp_offset;

    out << " subq $" << structs_info[e->name].size << ", %rsp" << endl;
    temp_offset -= structs_info[e->name].size;
    for(auto a : e->attrs) {
        ImpType imp_type = a->exp->accept(this);
        string type = imp_type.ttype;
        bool is_struct = isStruct(type);
        if(is_struct) {
            copyStruct(struct_offset, current_struct_offset + structs_info[e->name].offsets[a->name], false);
        } else {
            out << " movq %rax, " << current_struct_offset + structs_info[e->name].offsets[a->name] << "(%rbp)" << endl;
        }
    }
    struct_name = e->name;
    struct_offset = current_struct_offset;
    return ImpType(e->name);
}
```
Al evaluar un StructDec, se guardó toda su información en el mapa `structs_info`:
```cpp
void GenCodeVisitor::visit(StructDec* s){
    StructInfo struct_info;
    int offset_struct = 0;
    for(auto i : s->attrs) {
        struct_info.offsets[i->name] = offset_struct;
        struct_info.types[i->name] = ImpType(i->type);
        offset_struct -= getSize(i->type);
    }
    struct_info.size = -offset_struct;
    structs_info[s->name] = struct_info;
}
```
Al acceder a los atributos de un struct, se evalúa PostfixExp, que calcula el offset mediante el `struct_offset` y la posición del atributo guardad den `structs_info`:
```cpp
ImpType GenCodeVisitor::visit(PostfixExp* e) {
    ImpType imp_type = e->left->accept(this);
    string type = imp_type.ttype;
    ImpType return_imp_type = structs_info[type].types[e->right];
    return_imp_type.reference = imp_type.reference;
    string return_type = return_imp_type.ttype;

    bool is_struct = isStruct(return_type);
    if(is_struct) {
        struct_name = return_type;
        struct_offset = struct_offset + structs_info[type].offsets[e->right];
    } else {
        if(imp_type.reference) {
            out << " movq " << struct_offset + structs_info[type].offsets[e->right] << "(%rax), %rax" << endl;
        } else {
            out << " movq " << struct_offset + structs_info[type].offsets[e->right] << "(%rbp), %rax" << endl;
        }
    }
    return return_imp_type;
}
```
En caso el atributo sea otro struct, simplemente se actualiza struct_offset para que la siguiente llamada a PostfixExp lo use como base.

Al evaluar un FunctionCallExp y FunctionCallStatement, se pasan los valores a sus registros correspondientes, y en caso la variable sea un struct, se pasa su dirección absoluta. Además, si la función retorna un struct, se reserva memoria temporalmente, donde la función guardará el struct retornado:
```cpp
ImpType GenCodeVisitor::visit(FunctionCallExp* e) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int i = 0;
    for(auto it = e->argList.begin(); it != e->argList.end(); ++it) {
        ImpType imp_type = (*it)->accept(this);
        if(isStruct(imp_type.ttype)) {
            out << " movq %rbp, %rax" << endl;
            out << " addq $" << struct_offset << ", %rax" << endl;
            out << " movq %rax, " << argRegs[i] << endl;
        } else {
            out << " movq %rax, " << argRegs[i] << endl;
        }
        ++i;
    }
    string ret_type = functions_info[e->name].ttype;
    if(isStruct(ret_type)) {
        out << " subq $" << structs_info[ret_type].size << ", %rsp" << endl;
        struct_offset = temp_offset;
        temp_offset -= structs_info[ret_type].size;
    }
    out << " call " << e->name << endl;

    return functions_info[e->name];
}
```
Al asignar un struct a una variable, se usa el `struct_offset` que se calculó al aceptar el expression, que define dónde está el struct que vamos a asignar. Entonces solamente copiamos todos esos valores a la posición reservada de la variable. En caso el struct a asignar sea un referencia, se interpreta como una dirección, y se usa en vez del `%rbp`.
```cpp
void GenCodeVisitor::visit(AssignStatement* s) {
    assert(env.check(s->names[0]));
    ImpType imp_type = s->right->accept(this);
    bool is_struct = isStruct(imp_type.ttype);
    int offset_assign = get<1>(env.lookup(s->names[0]));
    int var_offset = offset_assign;
    string name = get<0>(env.lookup(s->names[0])).ttype;
    bool reference = get<0>(env.lookup(s->names[0])).reference;
    if(reference) {
        offset_assign = 0;
    }
    for(int i = 1; i < s->names.size(); ++i) {
        offset_assign = offset_assign + structs_info[name].offsets[s->names[i]];
        name = structs_info[name].types[s->names[i]].ttype;
    }
    switch (s->op) {
        case AS_ASSIGN_OP:
            if(is_struct) {
                out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                copyStruct(struct_offset, offset_assign, reference);
            } else {
                if(reference) {
                    out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                    out << " movq %rax, " << offset_assign << "(%rcx)" << endl;
                } else {
                    out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
                }
            }
            break;
        ...
    }
}
```

A la hora de copiar un struct, se usa la función `copyStruct()`, que copia todo un struct recursivamente de un origen a un destino:
```cpp
void GenCodeVisitor::copyStruct(int src_base, int dest_base, bool reference) {
    bool is_struct = isStruct(struct_name);
    if(!is_struct) {
        if(reference) {
            out << " movq " << src_base << "(%rbp), %rax" << endl;
            out << " movq %rax, " << dest_base << "(%rcx)" << endl;
        } else {
            out << " movq " << src_base << "(%rbp), %rax" << endl;
            out << " movq %rax, " << dest_base << "(%rbp)" << endl;
        }
        return;
    }

    string old_struct_name = struct_name;
    for(auto i : structs_info[struct_name].offsets) {
        struct_name = structs_info[struct_name].types[i.first].ttype;
        copyStruct(src_base + i.second, dest_base + i.second, reference);
        struct_name = old_struct_name;
    }
}
```

## Longs

### Scanner

### Parser

### Check Visitor

### GenCode Visitor

## Otras cosas

### Assign operations

#### Scanner


Se añadieron los siguientes tokens para manejar operaciones con assigns:

| Nombre | Token | Significado |
|--------|-------|-------------|
| PLUSASSIGN | += | Suma su valor y luego se asigna |
| MINUSASSIGN | -= | Resta su valor y luego se asigna |
| MULASSIGN | *= | Multiplica su valor y luego se asigna |
| DIVASSIGN | /= | Divide su valor y luego se asigna |
| MODASSIGN | %= | Calcula el residuo de su valor y luego se asigna |

#### Parser

Se modificó el AssignStatement para que acepte un igual (`=`) o cualquier de las nuevas operaciones:

```rust
Stmt ::= id (.id)* (= | += | -= | *= | /= | %=) AExp ;
```

#### Check Visitor

#### GenCode Visitor

Se modificó la evaluación del AssignStatement para que haga la operación correspondiente en caso haya un assign con operación. También se maneja el caso de que la operación se haga a un atributo pasado por referencia:

```cpp
void GenCodeVisitor::visit(AssignStatement* s) {
    
    ...

    switch (s->op) {
        case AS_ASSIGN_OP:
            if(is_struct) {
                out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                copyStruct(struct_offset, offset_assign, reference);
            } else {
                if(reference) {
                    out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                    out << " movq %rax, " << offset_assign << "(%rcx)" << endl;
                } else {
                    out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
                }
            }
            break;
        case AS_PLUS_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                out << " addq %rax, " << offset_assign << "(%rcx)" << endl;
            } else {
                out << " addq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_MINUS_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                out << " subq %rax, " << offset_assign << "(%rcx)" << endl;
            } else {
                out << " subq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_MUL_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rdx" << endl;
                out << " movq %rax, %rcx" << endl;
                out << " movq " << offset_assign << "(%rdx), %rax" << endl;
                out << " imulq %rcx, %rax" << endl;
                out << " movq %rax, " << offset_assign << "(%rdx)" << endl;
            } else {
                out << " movq %rax, %rcx" << endl;
                out << " movq " << offset_assign << "(%rbp), %rax" << endl;
                out << " imulq %rcx, %rax" << endl;
                out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_DIV_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rsi" << endl;
                out << " movq %rax, %rcx" << endl;
                out << " movq " << offset_assign << "(%rsi), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " movq %rax, " << offset_assign << "(%rsi)" << endl;
            } else {
                out << " movq %rax, %rcx" << endl;
                out << " movq " << offset_assign << "(%rbp), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_MOD_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rsi" << endl;
                out << " movq %rax, %rcx" << endl;
                out << " movq " << offset_assign << "(%rsi), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " movq %rdx, " << offset_assign << "(%rsi)" << endl;
            } else {
                out << " movq %rax, %rcx" << endl;
                out << " movq " << offset_assign << "(%rbp), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " movq %rdx, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        default:
            errorHandler.error("Not assignOp supported");
    }
}
```

### Break & Continue

#### Scanner

Se agregaron los siguientes tokens:

| Nombre | Token | Significado |
|--------|-------|-------------|
| BREAK | break | Keyword que indica un break |
| CONTINUE | continue | Keyword que indica un continue |

#### Parser



#### Check Visitor

#### GenCode Visitor

### Comentarios

#### Scanner
Los comentarios se manejan a nivel ede scanner