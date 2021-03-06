                                   ΠΡΟΓΡΑΜΜΑΤΙΣΤΙΚΗ ΑΣΚΗΣΗ 3

comptroller:
    Είναι ένα ανεξάρτητο πρόγραμμα στο οποίο αρχικοποιούνται οι τιμές που δίνονται ως 
παράμετροι σε περίπτωση που δεν δοθούν, αλλιώς ελέγχει τα flags κι αποθηκεύει τις ανάλογες 
τιμές. Ο comptroller στηρίζεται σε ένα loop το οποίο σταματά μόνο όταν η τιμή του γίνει 
0 όταν σταματάει δηλαδή κι το myport όπου μέσω του shared memory διαχειριζόμαστε αυτή την 
τιμή. Μέσα στο loop γίνεται sleep για ένα δευτερόλεπτο κι έτσι διασφαλίζουμε με έναν 
counter-timer τα δευτερόλεπτα που θέλουμε για να εκτυπώσουμε τα αποτελέσματα. Με τη βοήθεια 
του mod μπορούμε να δούμε αν τα δευτερόλεπτα είναι πολλαπλάσια από την τιμή που θέλουμε για
 να εκτυπώνουμε ανάλογα.

station_manager:
    Στον station manager γίνεται αρχικά attach memory για την διαχείριση των δεδομένων κι αυτό 
το πρόγραμμα στηρίζεται σε ένα loop όπως κι ο comptoller που σταματάει όταν σταματήσει το 
πρόγραμμα του myprog με μια μεταβλητή τύπου bool. Μέσα στο loop παίρνει τις τιμές των βασικών 
σηματοφόρων κι ανάλογα αν ο σηματοφόρος enter κι mantime_enter είναι 0, δηλαδή κανένα λεωφορείο
 δεν κάνει μανούβρα κι κανένα δεν μπαίνει, τότε αλλάζει την τιμή του σηματοφόρου ώστε να δώσει
 την πρόσβαση στο λεωφορείο που ζήτησε την πρόσβαση. Το αντίστοιχο γίνεται κι με τους σηματοφόρους
 exit και mantime_exit για τη διαδικασία της εξόδου.    
    
bus.c:
    Σε αυτό το ανεξάρτητο πρόγραμμα οι τιμές αρχικοποιούνται σε περίπτωση που δεν δοθούν ως παράμετροι.
 Επίσης δέχεται τις τιμές των σηματοφόρων με sem_wait() , sem_post() κι ανάλογα γίνονται οι αντίστοιχες
 ενέργειες όπως την εκτύπωση των μηνυμάτων στον public ledger την ανάλογη μείωση των διαθέσιμων
θέσεων στην νησίδα ενός τύπου λεωφορείου όταν παρκάρει κάποιο λεωφορείο εκεί. Αποβιβάζονται οι άνθρωποι
 κι προστίθενται άλλοι με τη χρήση της rand() η οποία χρησιμοποιεί την time.h για να είναι οι 
τιμές διαφορετικές κάθε φορά. Επίσης για τις διαδικασίες της μανούβρας κι της χρονικής διάρκειας που 
είναι παρκαρισμένο χρησιμοποιείται η sleep() με τους ανάλογους χρόνους. Για να βρεθεί ο χρόνος που χρειάστηκε
 για την διαδικασία για να τα χειριστεί ο comptroller χρησιμοποιεί τη gettimeofday(). Για να γίνει η 
εγγραφή των στοιχείων στο public ledger ανοίγεται ένα αρχείο για εγγραφή την πρώτη φορά κι έπειτα ανοίγεται
 το ίδιο για append() κι όλες οι εγγραφές γίνονται με την επίβλεψη του σηματοφόρου mutex για να μην υπάρχει
 ταυτόχρονη εγγραφή.

myport.c:
    Με το πρόγραμμα αυτό γίνεται η αρχικοποίηση των τιμών στο shared memory κι ανοίγεται τον configuration 
file. Για να επιτευχθεί η ταυτόχρονη εκτέλεση των πραγμάτων δημιουργούνται διεργασίες με fork() όπου σε μία
 καλείται με execvp ο station_manager, με την άλλη o comptroller κι στην άλλη δημιουργούνται τόσες διεργασίες
 όσα θα είναι κι τα λεωφορεία. Το πρόγραμμα τελειώνει όταν ο χρήστης του πατήσει το enter έτσι ώστε να καλύψει
 το getchar(). Κι επίσης γίνεται στο τέλος κι η αποδέσμευση μνήμης.

Σε όλα τα παραπάνω προγράμματα κάθε φορά που γίνεται επεξεργασία στην κοινή μνήμη διασφαλίζεται πως δεν 
υπάρχει περίπτωση overwriting μέσω της χρήσης του mutex.
