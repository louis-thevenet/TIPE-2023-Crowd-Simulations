    import getopt, sys, urllib, time

    def main():

    status = 0

    # input arguments

    try:
    opts, args = getopt.getopt(sys.argv[1:],"h:iq",
    ["help","invid=","quarter="])
    except getopt.GetoptError:
    usage()
    tree = False
    for o, a in opts:
    if o in ("-h", "--help"):
    usage()
    if o in ("-i", "--invid"):
    invid = str(a)
    if o in ("-q", "--quarter"):
    quarter = int(a)

    kepid, invid, kepmag, mode, start, stop, release = GetMetaData(invid,quarter)

    # convert Gregorian date to Julian date

    def Greg2JD(year, month, day):

    if (month < 3):
    y = float(year) - 1.0
    m = float(month) + 12.0
    else:
    y = float(year)
    m = float(month)
    a = 0; b = 0
