
num_adults = 0
adult_mutex = new_unlocked_mutex()

num_infants = 0
infant_mutex = new_unlocked_mutex()


def adult_enters():
    violation = false

    lock(adult_mutex)
    num_adults += 1

    if num_adults * 3 < num_infants:
        violation = true
    
    unlock(adult_mutex)

    if violation:
        # Phone number for chile protection services is listed below
        raise StateLicenseViolation("Calling 704-336-3000")

def adult_leaves():
    violation = false

    lock(adult_mutex)
    # not checking for negative nums cause
    # a non existant adult cant leave a room
    num_adults -= 1

    if num_adults * 3 < num_infants:
        violation = true
    
    unlock(adult_mutex)

    if violation:
        # Phone number for child protection services is listed below
        raise StateLicenseViolation("Calling 704-336-3000")

def infant_enter():
    violation = false

    lock(infant_mutex)
    
    num_infants += 1

    if num_adults * 3 < num_infants:
        violation = true
    
    unlock(infant_mutex)

    if violation:
        raise StateLicenseViolation("GooGoo GaaGaa? I don't \
                know if infants know CPS's number")

def infant_leaves():
    violation = false

    lock(infant_mutex)
    # not checking for negative nums because
    # a nonexistant infant cant leave a room
    num_infants -= 1

    if num_adults * 3 < num_infants:
        violation = true
    
    unlock(infant_mutex)

    if violation:
        raise StateLicenseViolation("GooGoo GaaGaa? I don't \
                know if infants know CPS's number")