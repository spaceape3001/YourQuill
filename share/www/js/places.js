
class Place extends Obj {
	constructor(arg = false)
	{
		super("/api/place", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.name;
    }
    
    brief(){
        return this.data.brief;
    }
};

function toPlaces(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Place(x));
    return ret;
}
