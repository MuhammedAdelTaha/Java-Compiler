#include "CFGParser.h"
#include "Utilities.h"
#include <iostream>
#include <queue>
#include <set>

// Function to print the grammar
//void printGrammar(const CFGParser::Grammar& grammar) {
//    for (const auto& [nonTerminal, productions] : grammar) {
//        std::cout << nonTerminal << " -> ";
//        for (size_t i = 0; i < productions.size(); ++i) {
//            const auto& prod = productions[i];
//            if (prod.empty()) {
//                std::cout << "epsilon"; // Represent epsilon
//            } else {
//                for (const auto& symbol : prod) {
//                    std::cout << symbol.name << " ";
//                }
//            }
//            if (i != productions.size() - 1) {
//                std::cout << "| ";
//            }
//        }
//        std::cout << "\n";
//    }
//    std::cout << "\n";
//}

// Test Case 1: Immediate Left Recursion
void testImmediateLeftRecursion() {
    std::cout << "=== Test Case 1: Immediate Left Recursion ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // A -> A a | b
    grammar["A"] = {
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "a"} },
        { Symbol{Symbol::Type::Terminal, "b"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 2: Indirect Left Recursion
void testIndirectLeftRecursion() {
    std::cout << "=== Test Case 2: Indirect Left Recursion ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // A -> B a | c
    grammar["A"] = {
        { Symbol{Symbol::Type::NonTerminal, "B"}, Symbol{Symbol::Type::Terminal, "a"} },
        { Symbol{Symbol::Type::Terminal, "c"} }
    };

    // B -> A b | d
    grammar["B"] = {
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "b"} },
        { Symbol{Symbol::Type::Terminal, "d"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 3: Both Immediate and Indirect Left Recursion
void testBothImmediateAndIndirectLeftRecursion() {
    std::cout << "=== Test Case 3: Both Immediate and Indirect Left Recursion ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> A a | b
    grammar["S"] = {
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "a"} },
        { Symbol{Symbol::Type::Terminal, "b"} }
    };

    // A -> S b | c
    grammar["A"] = {
        { Symbol{Symbol::Type::NonTerminal, "S"}, Symbol{Symbol::Type::Terminal, "b"} },
        { Symbol{Symbol::Type::Terminal, "c"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 4: Left Factoring
void testLeftFactoring() {
    std::cout << "=== Test Case 4: Left Factoring ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> A a | A b | c
    grammar["S"] = {
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "a"} },
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "b"} },
        { Symbol{Symbol::Type::Terminal, "c"} }
    };

    // A -> e | f
    grammar["A"] = {
        { Symbol{Symbol::Type::Terminal, "e"} },
        { Symbol{Symbol::Type::Terminal, "f"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 5: Combination of Left Recursion Elimination and Left Factoring
void testCombinationLeftRecursionAndFactoring() {
    std::cout << "=== Test Case 5: Combination of Left Recursion Elimination and Left Factoring ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // E -> E + T | E - T | T
    grammar["E"] = {
        { Symbol{Symbol::Type::NonTerminal, "E"}, Symbol{Symbol::Type::Terminal, "+"}, Symbol{Symbol::Type::NonTerminal, "T"} },
        { Symbol{Symbol::Type::NonTerminal, "E"}, Symbol{Symbol::Type::Terminal, "-"}, Symbol{Symbol::Type::NonTerminal, "T"} },
        { Symbol{Symbol::Type::NonTerminal, "T"} }
    };

    // T -> T * F | T / F | F
    grammar["T"] = {
        { Symbol{Symbol::Type::NonTerminal, "T"}, Symbol{Symbol::Type::Terminal, "*"}, Symbol{Symbol::Type::NonTerminal, "F"} },
        { Symbol{Symbol::Type::NonTerminal, "T"}, Symbol{Symbol::Type::Terminal, "/"}, Symbol{Symbol::Type::NonTerminal, "F"} },
        { Symbol{Symbol::Type::NonTerminal, "F"} }
    };

    // F -> ( E ) | id
    grammar["F"] = {
        { Symbol{Symbol::Type::Terminal, "("}, Symbol{Symbol::Type::NonTerminal, "E"}, Symbol{Symbol::Type::Terminal, ")"} },
        { Symbol{Symbol::Type::Terminal, "id"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 6: Multiple Left Factoring
void testMultipleLeftFactoring() {
    std::cout << "=== Test Case 6: Multiple Left Factoring ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> A a | A b | A c | d
    grammar["S"] = {
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "a"} },
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "b"} },
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "c"} },
        { Symbol{Symbol::Type::Terminal, "d"} }
    };

    // A -> e | f
    grammar["A"] = {
        { Symbol{Symbol::Type::Terminal, "e"} },
        { Symbol{Symbol::Type::Terminal, "f"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 7: No Left Recursion or Left Factoring Needed
void testNoLeftRecursionOrFactoring() {
    std::cout << "=== Test Case 7: No Left Recursion or Left Factoring Needed ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> a A | b B
    grammar["S"] = {
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::NonTerminal, "A"} },
        { Symbol{Symbol::Type::Terminal, "b"}, Symbol{Symbol::Type::NonTerminal, "B"} }
    };

    // A -> a | a S | b
    grammar["A"] = {
        { Symbol{Symbol::Type::Terminal, "a"} },
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::NonTerminal, "S"} },
        { Symbol{Symbol::Type::Terminal, "b"} }
    };

    // B -> b | b S | a
    grammar["B"] = {
        { Symbol{Symbol::Type::Terminal, "b"} },
        { Symbol{Symbol::Type::Terminal, "b"}, Symbol{Symbol::Type::NonTerminal, "S"} },
        { Symbol{Symbol::Type::Terminal, "a"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    ////printGrammar(transformedGrammar);
}

// Test Case 8.1: Grammar with Empty Productions (Epsilon)
void testEmptyProductions() {
    std::cout << "=== Test Case 8.1: Grammar with Empty Productions (Epsilon) ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> A a | ε
    grammar["S"] = {
        { Symbol{Symbol::Type::NonTerminal, "A"}, Symbol{Symbol::Type::Terminal, "a"} },
        { } // Represents ε (epsilon)
    };

    // A -> b
    grammar["A"] = {
        { Symbol{Symbol::Type::Terminal, "b"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    //printGrammar(transformedGrammar);
}

// Test Case 8.2: Grammar with Single Production
void testSingleProduction() {
    std::cout << "=== Test Case 8.2: Grammar with Single Production ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> a
    grammar["S"] = {
        { Symbol{Symbol::Type::Terminal, "a"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    //printGrammar(transformedGrammar);
}

// Test Case 9: Multiple Apostrophes in New Non-Terminals
void testMultipleApostrophes() {
    std::cout << "=== Test Case 9: Multiple Apostrophes in New Non-Terminals ===\n";

    // Define the grammar
    CFGParser::Grammar grammar;

    // S -> a a A | a a B | a b C | a b D | a c E | d
    grammar["S"] = {
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::NonTerminal, "A"} },
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::NonTerminal, "B"} },
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::Terminal, "b"}, Symbol{Symbol::Type::NonTerminal, "C"} },
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::Terminal, "b"}, Symbol{Symbol::Type::NonTerminal, "D"} },
        { Symbol{Symbol::Type::Terminal, "a"}, Symbol{Symbol::Type::Terminal, "c"}, Symbol{Symbol::Type::NonTerminal, "E"} },
        { Symbol{Symbol::Type::Terminal, "d"} }
    };

    // A -> e
    grammar["A"] = {
        { Symbol{Symbol::Type::Terminal, "e"} }
    };

    // B -> f
    grammar["B"] = {
        { Symbol{Symbol::Type::Terminal, "f"} }
    };

    // C -> g
    grammar["C"] = {
        { Symbol{Symbol::Type::Terminal, "g"} }
    };

    // D -> h
    grammar["D"] = {
        { Symbol{Symbol::Type::Terminal, "h"} }
    };

    // E -> i
    grammar["E"] = {
        { Symbol{Symbol::Type::Terminal, "i"} }
    };

    // Initialize the parser
    CFGParser parser(grammar);

    // Eliminate left recursion
    parser.eliminateLeftRecursion();

    // Perform left factoring
    parser.leftFactor();

    // Retrieve and print the transformed grammar
    CFGParser::Grammar transformedGrammar = parser.getGrammar();

    //printGrammar(transformedGrammar);
}

//int main() {
//    testImmediateLeftRecursion();
//    testIndirectLeftRecursion();
//    testBothImmediateAndIndirectLeftRecursion();
//    testLeftFactoring();
//    testCombinationLeftRecursionAndFactoring();
//    testMultipleLeftFactoring();
//    testNoLeftRecursionOrFactoring();
//    testEmptyProductions();
//    testSingleProduction();
//    testMultipleApostrophes();
//    return 0;
//}
