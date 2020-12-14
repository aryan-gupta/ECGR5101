
# This assumes that there is one barber,
# multiple waiting seats and multiple
# customers. Also assumes that its the
# barber that moves around to cut hair

# stores the number of customers ready and
# sitting in a chair waiting for a hair cut
counting_semaphore customerReady

# protects the chairs available from race
# conditions.
# we cant use a counting semaphore here
# because we cant bind how many threads
# are waiting. Though I may need more research 
# to verify this
mutex chairs
chairs_available = 3

# stores if the barber is done cutting hair
# used to signal to the customer that the 
# barber has finished cutting hair
binary_semaphore cut

def barber_function():
    while True:
        wait(customerReady)  # Wait until a customer is ready
        cutHair()  # cut their hair


def customer_function():
    leave = False
    
    # check if there are available chairs
    # to sit in
    lock(chairs)
    if (chairs_available == 0):
        leave = True
    else:
        chairs_available -= 1
    unlock(chairs)

    # leave if no seats are available
    if (leave):
        return
    
    # increment semaphore signaling another
    # customer is ready to get a hair cut
    signal(customerReady)
    getHairCut()

    # lock the chairs again and set it 
    # available for other people
    lock(chairs)
    chairs_available += 1
    unlock(chairs)

# cut hair, which takes 2 secs
# then signal to customer that
# we have finished cutting hair
def cutHair():
    sleep(2)
    signal(cut)

# wait for hair to be cut
# called by customer
def getHairCut():
    wait(cut)