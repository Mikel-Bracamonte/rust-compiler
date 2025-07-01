# Rust Compiler
Presentación: https://www.canva.com/design/DAGr5LTo40I/Lrq_WOXgYDGuzzKns2eicQ/edit?utm_content=DAGr5LTo40I&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton

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

#### Ambigüedades

Al parsear un StructExp, puede surgir una ambigüedad de la siguiente manera:

```rust
fn main() {
    let a: i32 = 1;
    let b: i32 = 2;
    if a < b {
        println!("{}", a);
    }
}
```

Al parsear la condición del IfExp, lo que sucede es que se toma el `b {`, como un StructExp, por lo que el siguiente código: `let a: i32 = 1;`, se parsea como si fuera un atributo de un struct, y falla la gramática.

A continuación se muestra otro ejemplo:

```rust
struct b {
    x: i32,
    y: i32
}

fn main() {
    let a: i32 = 1;
    if a < b { x: 1, y: 2 }.y {
        println!("{}", a);
    }
}
```

En este caso `b` es un struct, y `b { x: 1, y:2 }` es un StructExp, y en este caso se puede observar cómo a la hora de parsear de izquierda a derecha, no es posible saber si `b` es una variable o se está instanciando un struct.

Para solucionar esto, se usó una variable global `expect_struct`, que es falsa a la hora de parsear la condición en un if, while y for. Entonces, a la hora de parsear una expresión, si `expect_struct` es falsa y nos encontramos este caso ambiguo, se toma como un IdExp, en vez de un StructExp:

```cpp
else if (match(Token::IF)) {
    bool old_expect_struct = expect_struct;
    expect_struct = false;
    Exp* e1 = parseAExp();
    expect_struct = old_expect_struct;
    
    ...

}
```

En caso encontremos un paréntesis, esta ambigüedad queda resuelta, por lo que podemos poner `expect_struct` a verdadero:

```cpp
else if (match(Token::PI)) {
    bool old_expect_struct = expect_struct;
    expect_struct = true;
    e = parseAExp();
    expect_struct = old_expect_struct;
    
    ...

}
```

De esta forma, se asume que se está refirendo a un IdExp, y en caso se requiera un StructExp, se pueden añadir paréntesis de la sigueinte forma:

```rust
struct b {
    x: i32,
    y: i32
}

fn main() {
    let a: i32 = 1;
    if a < (b { x: 1, y: 2 }.y) {
        println!("{}", a);
    }
}
```

### Check Visitor

El check se asegura primero que no existan otros structs declarados. Se asegura que los tipos definidos para cada atributo fueron declarados previamente (en caso de tener)

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

El manejo de longs ya está hecho por el gencode pues todo los tipos usan el registro %rax que tiene 8 bytes.

### Parser

Ahora NumExp debe tener value de tipo long para poder guardar longs.

```cpp
class NumberExp : public Exp {
public:
    long long value;
    NumberExp(long long v);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~NumberExp();
};
```

### Check Visitor

Rust no acepta operaciones entre i32 e i64, por lo que es necesario manejar esa diferencia. Sin embargo podemos efectuar la operación entre una variable con valor númerico y un número. Por ejemplo:

```rust
fn main() {
    let mut id : i32 = 2;
    let mut id2 : i64 = 4;
    id = id + 4; // aceptado
    id2 = id2 + 8; // aceptado
    id += id2 // no aceptado, tipo distinto.
}
```

Por lo que cualquier valor que sea operado entre números no puede tener un tipo definido. Es por ello que definimos el tipo _0num_ para asignarle a los números.

Con ello se tiene la siguiente lógica:

```cpp
string CheckVisitor::getTypeOp(ImpType a, ImpType b) {
    if(a.ttype == b.ttype){
        return a.ttype;
    }
    if(b.ttype == "0num"){
        swap(a, b);
    }
    return b.ttype;
}
```

Esta función retorna el tipo que debe retornar entre dos ImpType. Además asume que pueden ser operados (este check lo realiza la función ```bool CheckVisitor::checkTypeOp(ImpType a, ImpType b)```). Por lo que devuelve el tipo 0num si ambos son 0num, o en todo caso retorna i32 o i64.

A pesar de ser un cambio simple, esta lógica debía ser realizada para todo el resto del código. (Tarea potente :'c)

### GenCode Visitor

Estuvo planeado cambiar los registros según el tipo de dato:

| Tipo | Assembly |
|--------|-------|
| bool | %al |
| i32 | %eax |
| i64 | %rax |

Sin embargo, esta era una tarea titánica para mantener el resto de la lógica funcional, por lo que no fue implementado.

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

### Injections

Puede ocurrir un problema a la hora de definir funciones. Para manejar labels en bucles y condicionales, usamos la nomenclatura de `{nombre del loop}_{numero}:`, por ejemplo: `while_0:`. Para las labels de las funciones, manejamos la nomenclatura `{nombre de la función}:`. Entonces, puede ocurrir un problema si nombramos a una función con un nombre que normalmente tendría el label de un loop. Por ejemplo, el siguiente código:

```rust
fn while_0() {
    let mut a: i32 = 0;
    while a < 10 {
        a += 1;
        println!("{}", a);
    }
    return;
}

fn main() {
    while_0();
}
```

Compila al siguiente código ensamblador:
```x86asm
.data
print_fmt_ln: .string "%ld \n"
print_fmt: .string "%ld "
.text
.globl while_0
while_0:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $0, %rax
 movq %rax, -8(%rbp)
while_0:
 movq -8(%rbp), %rax
 pushq %rax
 movq $10, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
  testq %rax, %rax
  je endwhile_0
 movq $1, %rax
 addq %rax, -8(%rbp)
 movq -8(%rbp), %rax
 movq %rax, %rsi
 leaq print_fmt_ln(%rip), %rdi
 movl $0, %eax
 call printf@PLT
  jmp while_0
endwhile_0:
jmp .end_fn_while_0
.end_fn_while_0:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 call while_0
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
```

Como se observa, hay dos tags `while_0:`, por lo que el código lanza un error.

Para solucionar esto, todas las funciones internamente empiezan con un prefijo `fn_`, de manera que ningún otro label pueda tener el mismo nombre. El mismo código de antes en las partes problemáticas quedaría de la siguiente manera:

```x86asm
.data
print_fmt_ln: .string "%ld \n"
print_fmt: .string "%ld "
.text
.globl fn_while_0
fn_while_0:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $0, %rax
 movq %rax, -8(%rbp)
while_0:
 movq -8(%rbp), %rax
 pushq %rax
 movq $10, %rax
 movq %rax, %rcx

 ...

 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 call fn_while_0
.end_main:

 ...
```

### Break & Continue

#### Scanner

Se agregaron los siguientes tokens:

| Nombre | Token | Significado |
|--------|-------|-------------|
| BREAK | break | Keyword que indica un break |
| CONTINUE | continue | Keyword que indica un continue |

#### Parser

Se agregaron break y continue como Statements:
```rust
Stmt ::= break ; |
    continue ;
```

#### Check Visitor

No se aceptan breaks ni continues fuera de un loop.

#### GenCode Visitor

Al entrar a un loop, actualizamos un stack `nombreLoop`, de modo que en todo momento guarda el nombre del loop en el que estamos. De ese modo, al querer hacer un break, simplemente saltamos al end de ese nombre, y si queremos hacer continue, saltamos al inicio de se nombre:

```cpp
void GenCodeVisitor::visit(BreakStatement* s) {
    assert(!nombreLoop.empty());
    out << "jmp end" << nombreLoop.top() << endl;
}

void GenCodeVisitor::visit(ContinueStatement* s) {
    assert(!nombreLoop.empty());
    out << "jmp " << nombreLoop.top() << endl;
}
```

### Comentarios

#### Scanner
Los comentarios se manejan a nivel del scanner. Para los comentarios de una linea, si se lee un `/`seguido de otro `/`, se ignoran todos los caracteres hasta llegar a un salto de línea o hasta que acabe el input. En caso se lea un `/` seguido de un `*`, se suma uno a `comment_depth`, y al leer un `*` seguido de un `/` se resta uno a `comment_depth`. Mientras `comment_depth` sea mayor a 0, se ignoran todos los caracteres:

```cpp
Token* Scanner::nextToken() {
    
    ...
    
    int comment_depth = 0;
    if(input[current] == '/') {
        if(current + 1 < input.length() && input[current + 1] == '/') {
            while(input[current] != '\n') {
                if(current >= input.length()) {
                    return new Token(Token::END);
                }
                current++;
            }
        } else if(current + 1 < input.length() && input[current + 1] == '*') {
            comment_depth++;
            current += 2;
            while(comment_depth > 0) {
                if(current >= input.length()) {
                    return new Token(Token::END);
                }
                if(input[current] == '/' && current + 1 < input.length() && input[current + 1] == '*') {
                    comment_depth++;
                    current++;
                } else if(input[current] == '*' && current + 1 < input.length() && input[current + 1] == '/') {
                    comment_depth--;
                    current++;
                }
                current++;
            }
        }
    }

    ...

}
```
