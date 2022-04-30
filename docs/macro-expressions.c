// Just use 'gcc -E <thisfilename>' to see the output.
//
// Beware, this file will not compile as a C program.
//         It is just to see how macros expand
//         
// 
// For a documentation see 
//       https://gcc.gnu.org/onlinedocs/cpp/Macros.html#Macros
//
//
//  ALWAYS
//   0. Macro definition is setting up a Name <-> Expansion associaiton
//   1. Macro usage causes substituting Name by Expansion
//   2. MACRO is expanded only on usage; during definition it is just kept as is
//   3. After expansion again check if other macros expansions apply
//   4. Dont expand inside quotes (strings)
// In the comments below the arrows '->' are used to indicate 
// immediate macro expansion
// There are two types of macros
// TYPE 1 object-like macros
// Expansion = simple replacement
// 
#define X 100 
o1. X		// X -> 100
#define BIG 1
o2. BIG		// BIG -> 1
#define MYBIG BIG
o3. MYBIG	// MYBIG -> BIG -> 1  (Note two step process)
#define A B	// Note that B is undefined
#define B 1
ooo. A		// A -> B -> 1
#define L_BIG 2	 // L_BIG -> 2


// TYPE 2 function-like macros
// Now expansion is in two ordered steps
// 	step 1. Expand all args 
//      step 2. Expanded macro with args substituted.
//  Exceptions to this order are when # and ## are used in the expansion
#define NAMEIT(X)   X	
f1. NAMEIT(BIG)	// NAMEIT(BIG) -> NAMEIT(1) -> 1   Arg expands first
#define DAY(X) "Today is " X 
f2. DAY(X) 	//-> DAY(100) -> "Today is " 100   Arg expands first

// Exception 1: In case of # on RHS of function-like
//                #X => "X" without further expansion of X
// Note as an exception Arg NOT expanded first if stringized!
#define DDAY(N) "Today is " #N 
e1. DDAY(X) 	// DDAY(X)
		// -> "Today is " #X  	Not the arg not expanded
		// -> "Today is " "X"

// Therefore to expand and quote, take two steps:
#define EDDAY(N) DDAY(N)
e2. EDDAY(X) 	// EDDAY(X) 
	  	// -> EDDAY(100) ->DDAY(100) -> "Today is " #100 
	  	// ->"Today is " "100"

// What happens to string around a string ??
#define STRINGIT(X) #X
e3. STRINGIT(BIG)	// -> #BIG ->"BIG"      expand before arg expansion
#define EXPSTRINGIT(X) STRINGIT(X) 
e4. EXPSTRINGIT(BIG)	// -> EXPSTRINGIT ( 1 ) 
		   	// -> STRINGIT(1) -> #1 -> "1"
#define QEXPSTRINGIT(X) EXPSTRINGIT(STRINGIT(X))
e5. QEXPSTRINGIT(BIG)
				// QEXPSTRINGIT(BIG)
				// -> QEXPSTRINGIT(1)
				// -> EXPSTRINGIT(STRINGIT(1))
				// -> EXPSTRINGIT( "1" )
				// -> STRINGIT("1")
				// -> #"1"
				// -> "\"1\""   : Notice nice escaping 
				// 		  of inner quote marks
	
// Eception 2: ## is used for concatenation, again DONT expand argument first.
#define CON(X)    L_##X 
e6. CON(BIG) 	// CON(BIG) 
		// ->L_BIG
		// ->2


// What if argument X occurs with and without #?
// first treat it as an exception and expand macro first
#define MYDAY(N) #N N 
e7. MYDAY(X) 	//  -> #X X       expand body first
		//  -> "X" 100
		
