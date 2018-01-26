# Iterative-Deepening-Radix-Stemmer
IDRS is an algorithm that uses a Trie structure to infeer stem of words


# Definitions
- **Son** : next node on trie.
- **Grandson** : next node of the next node on trie;
- **Grandfather Node** : a node which has only one grandson.
- **Father Node** : a node which has son(s).
- **C** : the **c**urrent node on interaction.
- **S** : the **s**on to be visited (the son you want to go to).
- **Esoteric Average (EA)** : (_TODO_ - explain)
# Examples regarding definition

```syntree      
        

        GRANDFATHER NODE
                 |
                / \
               /   \
              /     \
             /       \
            /         \
            |         |
        SON NODE    SON NODE(ALSO FATHER NODE)
                      |
                     / \
                    /   \
                   /     \
            SON NODE  SON NODE
```

# Algorithm

Order of check : 

- S doesn't exist? return C;

- S is final? return S;

- C is grandfather? next iteraction;

- S(p) >= C(EA)? next iteraction;

- S(p) < C(EA)? return S;