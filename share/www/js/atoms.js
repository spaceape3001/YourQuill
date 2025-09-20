
class Atom extends Obj {
	constructor(arg = false)
	{
		super("/api/atom", arg);
	}
};


function toAtoms(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Atom(x));
    return ret;
}
