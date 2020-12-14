# the signal that this thread is ok
# to board
conditional_var waiting_hackers 
conditional_var waiting_employees

# this mutex protects the num_* variables
mutex num

# the current number of hackers and employees
# waiting to board
num_hackers
num_employees

# This variable is needed because 
# there exists a execution path where the
# boart driver thread is waiting for the
# num_* counters to decrement and another
# hacker or employee joins the beach. It
# cant differentiate them and it waits
# for the number to decrement, which
# it never does
counting_semaphore num_on_boat

def RowBoat():
    thread.exit()

# 
def HackerArrives():
    # This variable stores if this thread is
    # boat driver or not. The boat driver
    # signals the other threads who is able
    # to board and starts the boat motor once
    # everyone is on board
    captain = False

    lock(num)   
    # The newcoming thread checks if the boat can take
    # off. If there are 4 of us (num_hackers >= 4) then
    # we can signal 3 of our hacker friends and take off
    # also if there are more than 2 employees and more 
    # than 1 other hacker we can also take off. If we 
    # find either of these conditions, we have become
    # the boat driver and will signal and wait for the 
    # people to board the boat. If we dont then we just
    # increment the waiting counter and wait on the
    # conditional variable
    if (num_hackers >= 3):
        # assume captains duty
        captain = True

        # wait until everyone is on the boat. By setting
        # the num_on_boat variable, we are essentially
        # locking the boat and preventing anyone from
        # assuming captains duties while we are boarding
        num_on_boat = 3

        # signal 3 of our hacker friends to board
        # the boat
        for i in range(3):
            signal(waiting_hackers)

        # wait on the semaphore to reach 0
        wait(num_on_boat)

    elif (num_employees >= 2 and num_hackers >= 1):
        captain = True

        num_on_boat = 3

        for i in range(1):
            signal(waiting_hackers)
        for i in range(2):
            signal(waiting_employees)

        wait(num_on_boat)

    else:
        num_hackers += 1
    unlock(num)

    if (captain):
        RowBoat()
    else:
        wait(waiting_hackers)
        num_on_boat -= 1

# the code is the same as above, except *_employees is
# swapped with *_hackers
def EmployeeArrives():
    captain = False

    lock(num)   
    if (num_employees >= 3):
        captain = True
        num_on_boat = 3

        for i in range(3):
            signal(waiting_employees)

        wait(num_on_boat)

    elif (num_hackers >= 2 and num_employees >= 1):
        captain = True

        num_on_boat = 3

        for i in range(1):
            signal(waiting_employees)
        for i in range(2):
            signal(waiting_hackers)

        wait(num_on_boat)
        
    else:
        num_hackers += 1
    unlock(num)

    if (captain):
        RowBoat()
    else:
        wait(waiting_employees)
        num_on_boat -= 1