# Logic

# SocialGaming

1. High Level Language (SocialGaming)
    * SocialGaming is a custom programming language for our platform that allows game creator to make the games that they want to play.
    * As a game creator, all you have to do is provide the source code for your game (written in SocialGaming), and the rest will be handled by our platform.

2. Lexical Analyzer (Lexer)
    * The lexer reads the source code characters and tokenize it into types and values. (i.e. 1 has a type of INT and a value of 1)
    * Remove all comments for the source code. See the [grammar](https://github.com/nsumner/tree-sitter-socialgaming) for SocialGaming for more information.
    * The lexer will also handle lexical errors (i.e. catch errors for unexpected characters)

3. Syntax Analyzer (Parser)
    * The parser creates a syntax tree from the source code use the tokens created by the lexer.
    *  Using the [cpp wrapper for tree-sitter](https://github.com/nsumner/cpp-tree-sitter/tree/main) and the [SocialGaming language](https://github.com/nsumner/tree-sitter-socialgaming), create a syntax tree
    The parser will also handle syntax errors (i.e. invalid operations)

5. Interpreter [What we have to implement]
    * The interpreter will traverse through the syntax tree and determine what code needs to be executed. The interpreter will handle the operations outlined in the [project description](https://coursys.sfu.ca/2024fa-cmpt-373-d1/pages/ProjectDescription)  

## Contributions
Logic tasks to be completed can be found [here](https://docs.google.com/spreadsheets/d/15BgfKZ0t4bjygdsNd4OhbN5z_UL_hawaG-xCbc36_p4/edit?usp=sharing) 

### Rule-Based Tasks
These set of tasks will all follow a similar development flow:

1. Implement a rule that uses the `IRule` and `BaseRuleSpecification` interface
2. Update the `TSParser` such that the parser will use the extended `BaseRuleSpecification` object implemented in step 1
3. Add tests for parsing the rule
4. Add tests for executing the rule through the interpreter

### Non-Rule-Based Tasks
These tasks have more ambiguity and therefore does not have a defined contribution guideline. 

## Helpful References
* https://www.youtube.com/watch?v=Eythq9848Fg&list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD&ab_channel=CodePulse