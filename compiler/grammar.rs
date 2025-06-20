Program ::= (fn FunDec)+

FunDec ::= id ([ParamDecList]) [-> Type] { Body }

ParamDecList ::= ParamDec (, ParamDec )*

ParamDec ::= id : [ mut ] Type

Type ::= id

Body ::= StmtList

StmtList ::= ( Stmt )*

Stmt ::= id (= | += | -= | *= | /= | %=) AExp ; |
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

Term ::= Factor ((*|/|%) Factor)*

Factor ::= [-] (id | Num | Bool | ( AExp ) | if AExp {AExp} else {AExp} | id ( [ArgList] ))

ArgList ::= AExp (, AExp)*

Bool ::= true | false
